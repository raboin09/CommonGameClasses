// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AbilityComponent.h"

#include "API/Ability/ActivationMechanism.h"
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
	const TScriptInterface<IAbility> NewEquippedAbility = Internal_FindAbility(NewEquippedSlot);
	if(!NewEquippedAbility)
	{
		return;
	}

	if(NewEquippedSlot == EquippedSlot)
	{
		return;
	}
	
	if(const TScriptInterface<IAbility> LastEquippedAbility = Internal_FindAbility(EquippedSlot))
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

void UAbilityComponent::AddAbilityFromClassInSlot(TSubclassOf<AActor> AbilityClass, const FGameplayTag& SlotTag)
{
	const TScriptInterface<IAbility> SpawnedAbility = Internal_SpawnAbilityFromClass(AbilityClass);
	Internal_InitAndAttachAbilityToOwnerMesh(SpawnedAbility);
	Internal_RemoveAbilityInSlot(SlotTag);
	Internal_AddAbilityInSlot(SlotTag, SpawnedAbility);
	if(SlotTag == EquippedSlot)
	{
		SpawnedAbility->EquipAbility();	
	}
}

void UAbilityComponent::TryStartAbilityInSlot(const FGameplayTag& SlotTag)
{
	const TScriptInterface<IAbility> Ability = Internal_FindAbility(SlotTag);
	if(!Ability)
	{
		return;
	}
	Ability->TryStartAbility();
}

void UAbilityComponent::TryStopAbilityInSlot(const FGameplayTag& SlotTag)
{
	const TScriptInterface<IAbility> Ability = Internal_FindAbility(SlotTag);
	if(!Ability)
	{
		return;
	}
	Ability->TryEndAbility();
}

void UAbilityComponent::TryActivateAwaitingMechanism(bool bShouldActivate) const
{
	const TScriptInterface<IActivationMechanism> MechanismToActivate = AwaitingActivationDetails.MechanismAwaitingActivation;
	if(!MechanismToActivate)
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

TScriptInterface<IAbility> UAbilityComponent::Internal_SpawnAbilityFromClass(TSubclassOf<AActor> AbilityClass) const
{
	if(!AbilityClass || !AbilityClass->ImplementsInterface(UAbility::StaticClass()))
	{
		return nullptr;
	}
	AActor* AbilityObj = UCommonWorldUtils::SpawnActorToCurrentWorld_Deferred<AActor>(AbilityClass, GetOwner(), Cast<APawn>(GetOwner()));
	UCommonWorldUtils::FinishSpawningActor_Deferred(AbilityObj, GetOwner()->GetTransform());
	return AbilityObj;
}

void UAbilityComponent::Internal_InitAndAttachAbilityToOwnerMesh(TScriptInterface<IAbility> AbilityToAttach) const
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

void UAbilityComponent::Internal_DestroyAbility(TScriptInterface<IAbility> AbilityToRemove)
{
	if(!AbilityToRemove)
	{
		return;
	}
	AbilityToRemove->UnEquipAbility();
	AbilityToRemove->DestroyAbility();
}

void UAbilityComponent::Internal_AddAbilityInSlot(const FGameplayTag& SlotTag, TScriptInterface<IAbility> AbilityToAdd)
{
	SlottedAbilities.Add(SlotTag, AbilityToAdd);
}

void UAbilityComponent::Internal_RemoveAbilityInSlot(const FGameplayTag& SlotTag)
{
	const TScriptInterface<IAbility> AbilityToRemove = Internal_FindAbility(SlotTag);
	if(!AbilityToRemove)
	{
		return;
	}
	Internal_DestroyAbility(AbilityToRemove);
	SlottedAbilities.Remove(SlotTag);
}

TScriptInterface<IAbility> UAbilityComponent::Internal_FindAbility(const FGameplayTag& SlotTag) const
{
	if(!HasAbilityInSlot(SlotTag))
	{
		return nullptr;
	}
	return *SlottedAbilities.Find(SlotTag);
}
