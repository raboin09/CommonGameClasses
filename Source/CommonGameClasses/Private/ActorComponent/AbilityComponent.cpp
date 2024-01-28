// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AbilityComponent.h"

#include "API/Ability/Ability.h"
#include "API/Ability/ActivationMechanism.h"
#include "Core/CommonAssetManager.h"
#include "GameFramework/Character.h"
#include "Utils/CommonWorldUtils.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityComponent::DestroyAbilities()
{
	for(auto KeyVal : SlottedAbilities)
	{
		Internal_RemoveAbilityInSlot(KeyVal.Key);
	}
}

void UAbilityComponent::SetCurrentEquippedSlot(const FGameplayTag& NewEquippedSlot)
{
	const TWeakInterfacePtr<IAbility> NewEquippedAbility = Internal_FindAbility(NewEquippedSlot);
	if(NewEquippedAbility.IsStale())
	{
		return;
	}

	if(NewEquippedSlot == EquippedSlot)
	{
		return;
	}

	const TWeakInterfacePtr<IAbility> LastEquippedAbility = Internal_FindAbility(EquippedSlot);
	if(!LastEquippedAbility.IsStale())
	{
		LastEquippedAbility->UnEquipAbility();
	}
	EquippedSlot = NewEquippedSlot;
	NewEquippedAbility->EquipAbility();	
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityComponent::AddAbilityFromClassInSlot(TSoftClassPtr<AActor> AbilityClass, const FGameplayTag& SlotTag)
{
	const TSubclassOf<AActor> ClassToSpawn = UCommonAssetManager::Sync_GetSubclass(AbilityClass);
	const TWeakInterfacePtr<IAbility> SpawnedAbility = Internal_SpawnAbilityFromClass(ClassToSpawn);
	if(!SpawnedAbility.IsValid())
	{
		return;
	}
	Internal_InitAndAttachAbilityToOwnerMesh(SpawnedAbility);
	if(HasAbilityInSlot(SlotTag))
	{
		Internal_RemoveAbilityInSlot(SlotTag);	
	}
	Internal_AddAbilityInSlot(SlotTag, SpawnedAbility);
	if(SlotTag == EquippedSlot)
	{
		SpawnedAbility->EquipAbility();	
	}
}

void UAbilityComponent::TryStartAbilityInSlot(const FGameplayTag& SlotTag)
{
	const TWeakInterfacePtr<IAbility> Ability = Internal_FindAbility(SlotTag);
	if(!Ability.IsValid())
	{
		return;
	}
	Ability->TryStartAbility();
}

void UAbilityComponent::TryStopAbilityInSlot(const FGameplayTag& SlotTag)
{
	const TWeakInterfacePtr<IAbility> Ability = Internal_FindAbility(SlotTag);
	if(!Ability.IsValid())
	{
		return;
	}
	Ability->TryEndAbility();
}

void UAbilityComponent::TryActivateAwaitingMechanism(bool bShouldActivate) const
{
	const TWeakInterfacePtr<IActivationMechanism> MechanismToActivate = AwaitingActivationDetails.MechanismAwaitingActivation;
	if(MechanismToActivate.IsStale())
	{
		return;
	}

	if(bShouldActivate)
	{
		FTriggerEventPayload MontageActivationPayload;
		MontageActivationPayload.bStartActivationImmediately = true;
		MontageActivationPayload.ActivationLevel = AwaitingActivationDetails.ActivationLevel;
		MechanismToActivate->Activate(MontageActivationPayload);	
	} else
	{
		MechanismToActivate->Deactivate();		
	}	
}

TWeakInterfacePtr<IAbility> UAbilityComponent::Internal_SpawnAbilityFromClass(TSubclassOf<AActor> AbilityClass) const
{
	if(!AbilityClass || !AbilityClass->ImplementsInterface(UAbility::StaticClass()))
	{
		return nullptr;
	}
	AActor* AbilityObj = UCommonWorldUtils::SpawnActorToCurrentWorld_Deferred<AActor>(AbilityClass, GetOwner(), Cast<APawn>(GetOwner()));
	UCommonWorldUtils::FinishSpawningActor_Deferred(AbilityObj, GetOwner()->GetTransform());
	return AbilityObj;
}

void UAbilityComponent::Internal_InitAndAttachAbilityToOwnerMesh(TWeakInterfacePtr<IAbility> AbilityToAttach) const
{
	if(const ACharacter* CharOwner = Cast<ACharacter>(GetOwner()))
	{
		AbilityToAttach->InitAbility(CharOwner->GetMesh());
	} else if(const APawn* PawnOwner = Cast<ACharacter>(GetOwner()))
	{
		if(UMeshComponent* MeshComp = PawnOwner->FindComponentByClass<UMeshComponent>())
		{
			AbilityToAttach->InitAbility(MeshComp);
		}
	}
}

void UAbilityComponent::Internal_DestroyAbility(TWeakInterfacePtr<IAbility> AbilityToRemove)
{
	if(AbilityToRemove.IsStale())
	{
		return;
	}
	AbilityToRemove->UnEquipAbility();
	AbilityToRemove->DestroyAbility();
}

void UAbilityComponent::Internal_AddAbilityInSlot(const FGameplayTag& SlotTag, TWeakInterfacePtr<IAbility> AbilityToAdd)
{
	if(AbilityToAdd.IsValid())
	{
		SlottedAbilities.Add(SlotTag, AbilityToAdd);	
	}	
}

void UAbilityComponent::Internal_RemoveAbilityInSlot(const FGameplayTag& SlotTag)
{
	const TWeakInterfacePtr<IAbility> AbilityToRemove = Internal_FindAbility(SlotTag);
	if(AbilityToRemove.IsStale())
	{
		return;
	}
	Internal_DestroyAbility(AbilityToRemove);
	SlottedAbilities.Remove(SlotTag);
}

TWeakInterfacePtr<IAbility> UAbilityComponent::Internal_FindAbility(const FGameplayTag& SlotTag) const
{
	if(!HasAbilityInSlot(SlotTag))
	{
		return nullptr;
	}
	return *SlottedAbilities.Find(SlotTag);
}
