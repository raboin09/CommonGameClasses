// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AbilityComponent.h"

#include "API/Ability/ActivationMechanism.h"
#include "GameFramework/Character.h"
#include "Utils/CommonWorldUtils.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityComponent::AddAbilityFromClassInSlot(TSubclassOf<AActor> AbilityClass, const FGameplayTag& SlotTag)
{
	if(!AbilityClass || !AbilityClass->ImplementsInterface(UAbility::StaticClass()))
	{
		return;
	}
	AActor* AbilityObj = UCommonWorldUtils::SpawnActorToPersistentWorld_Deferred<AActor>(AbilityClass, GetOwner(), Cast<APawn>(GetOwner()));
	UCommonWorldUtils::FinishSpawningActor_Deferred(AbilityObj, GetOwner()->GetTransform());
	TScriptInterface<IAbility> SpawnedAbility;
	SpawnedAbility.SetObject(AbilityObj);
	SpawnedAbility.SetInterface(Cast<IAbility>(AbilityObj));
	SlottedAbilities.Add(SlotTag, SpawnedAbility);
	if(const ACharacter* CharOwner = Cast<ACharacter>(GetOwner()))
	{
		SpawnedAbility->InitAbility(CharOwner->GetMesh());
	} else if(const APawn* PawnOwner = Cast<ACharacter>(GetOwner()))
	{
		if(UMeshComponent* MeshComp = PawnOwner->FindComponentByClass<UMeshComponent>())
		{
			SpawnedAbility->InitAbility(MeshComp);
		}
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

void UAbilityComponent::TryActivateAwaitingMechanism(bool bShouldActivate)
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

TScriptInterface<IAbility> UAbilityComponent::Internal_FindAbility(const FGameplayTag& SlotTag)
{
	if(!SlottedAbilities.Contains(SlotTag))
	{
		return nullptr;
	}
	return *SlottedAbilities.Find(SlotTag);
}
