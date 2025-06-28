﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AbilityComponent.h"

#include "API/Ability/Ability.h"
#include "API/Ability/ActivationMechanism.h"
#include "Core/ActorSystems/CommonSpawnSubsystem.h"
#include "GameFramework/Character.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityComponent::DestroyAbilities()
{
	TArray<FGameplayTag> AbilityKeys;
	SlottedAbilities.GetKeys(AbilityKeys);
	for(const FGameplayTag& Key : AbilityKeys)
	{
		Internal_RemoveAbilityInSlot(Key);
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
	AbilityEquipped.Broadcast(FNewAbilityEquippedPayload(NewEquippedAbility));
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityComponent::PostComponentLoadedFromSave()
{
	// AbilityClasses is added to during the loop, so use a copy
	TMap<FGameplayTag, TSoftClassPtr<AActor>> CopiedMap = AbilityClasses;
	for(auto KeyVal : CopiedMap)
	{
		AddAbilityFromClassInSlot(KeyVal.Value, KeyVal.Key);
	}
}

void UAbilityComponent::AddAbilityFromClassInSlot(TSoftClassPtr<AActor> AbilityClass, const FGameplayTag& SlotTag)
{
	const TSubclassOf<AActor> ClassToSpawn = AbilityClass.LoadSynchronous();
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
	Internal_AddAbilityInSlot(SlotTag, SpawnedAbility, ClassToSpawn);
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

void UAbilityComponent::TryActivateAwaitingMechanism(bool bShouldActivate)
{
	const TWeakInterfacePtr<IActivationMechanism> MechanismToActivate = AwaitingActivationDetails.MechanismAwaitingActivation;
	if(!MechanismToActivate.IsValid())
	{
		ResetAwaitingActivationDetails();
		return;
	}

	if(bShouldActivate)
	{
		AwaitingActivationDetails.TriggerActivationPayload.bStartActivationImmediately = true;
		MechanismToActivate->Activate(AwaitingActivationDetails.TriggerActivationPayload);	
	} else
	{
		MechanismToActivate->Deactivate();		
	}	
}

TWeakInterfacePtr<IAbility> UAbilityComponent::Internal_SpawnAbilityFromClass(const TSubclassOf<AActor>& AbilityClass) const
{
	if(!AbilityClass || !AbilityClass->ImplementsInterface(UAbility::StaticClass()))
	{
		return nullptr;
	}
	AActor* AbilityObj = UCommonSpawnSubsystem::SpawnActorToCurrentWorld_Deferred<AActor>(this, AbilityClass, GetOwner(), Cast<APawn>(GetOwner()));
	UCommonSpawnSubsystem::FinishSpawningActor_Deferred(AbilityObj, GetOwner()->GetTransform());
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

void UAbilityComponent::Internal_AddAbilityInSlot(const FGameplayTag& SlotTag, TWeakInterfacePtr<IAbility> AbilityToAdd, const TSubclassOf<AActor>& AbilityClass)
{
	if(AbilityToAdd.IsValid())
	{
		SlottedAbilities.Add(SlotTag, AbilityToAdd);
		AbilityClasses.Add(SlotTag, TSoftClassPtr<AActor>(AbilityClass));
		if(SlotTag == CommonGameSlot::SlotMain)
		{
			SetCurrentEquippedSlot(CommonGameSlot::SlotMain);
		}
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
