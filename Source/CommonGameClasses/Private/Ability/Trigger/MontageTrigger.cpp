#include "Ability/Trigger/MontageTrigger.h"

#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/LockOnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Types/CommonTagTypes.h"

void UMontageTrigger::InitTriggerMechanism()
{
	K2_HandleInitTrigger();
	CharacterAnimationComponent = GetInstigator()->FindComponentByClass<UCharacterAnimationComponent>();
	check(CharacterAnimationComponent.IsValid())
}

void UMontageTrigger::HandleSuccessfulTriggerPressed()
{
	if(bHasCombos)
	{
		if(UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameAbilityEvent::ComboActivated))
		{
			Internal_IncrementComboCounter();
		} else
		{
			Internal_ResetComboCounter();
		}
	}
	K2_HandleBeforePressedTrigger();
	Internal_StartMontage();
	K2_HandleAfterPressedTrigger();
	FTriggerEventPayload PressTriggerEventPayload;
	PressTriggerEventPayload.ActivationLevel = bHasCombos ? ComboSectionIncrement : 1;
	// Activation waits for montage notify to start
	PressTriggerEventPayload.bStartActivationImmediately = false;
	PressTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerPressedEvent.Broadcast(PressTriggerEventPayload);
}

void UMontageTrigger::HandleTriggerReleased()
{
	FTriggerEventPayload ReleaseTriggerEventPayload;
	ReleaseTriggerEventPayload.ActivationLevel = -1;
	// We wait for the montage notify to active the weapon
	ReleaseTriggerEventPayload.bStartActivationImmediately = false;
	ReleaseTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerReleasedEvent.Broadcast(ReleaseTriggerEventPayload);
	K2_HandleReleasedTrigger();
}

void UMontageTrigger::ResetTrigger()
{
	if(bHasCombos)
	{
		Internal_ResetComboCounter();
		TArray<FGameplayTag> StateTagsToRemove;
		StateTagsToRemove.Add(CommonGameAbilityEvent::ComboActivated);
		StateTagsToRemove.Add(CommonGameAbilityEvent::Active);
		StateTagsToRemove.Add(CommonGameAbilityEvent::Activated);
		UGameplayTagComponent::RemoveTagsFromActor(GetOwner(), StateTagsToRemove);	
	}
}

void UMontageTrigger::HandleMontageEnded(const FCharacterMontageEndedPayload& CharacterMontageEndedPayload)
{
	if(CharacterMontageEndedPayload.EndedMontage.Get() == MontageToPlay.Get() && CharacterMontageEndedPayload.UpcomingMontage.Get() != MontageToPlay.Get())
	{
		ResetTrigger();
	}
	CharacterAnimationComponent->OnCharacterMontageEnded().RemoveDynamic(this, &ThisClass::HandleMontageEnded);
}

FAnimMontagePlayData UMontageTrigger::Internal_GetPlayData() const
{
	FAnimMontagePlayData PlayData;	
	PlayData.MontageToPlay = MontageToPlay.Get();
	PlayData.MontageSection = K2N_GetNextMontageSection();
	return PlayData;
}

FName UMontageTrigger::K2N_GetNextMontageSection_Implementation() const
{
	if(bRandomizeMontageSection)
	{
		return FName(ComboPrefix + FString::FromInt(UKismetMathLibrary::RandomIntegerInRange(1, MaxComboSections)));
	}
	return FName(ComboPrefix + FString::FromInt(ComboSectionIncrement));
}

void UMontageTrigger::Internal_StartMontage()
{
	const APawn* CurrentInstigator = GetInstigator();
	if(!CurrentInstigator)
	{
		return;
	}
	
	if(!CharacterAnimationComponent.IsStale())
	{
		const FAnimMontagePlayData PlayData = Internal_GetPlayData();
		CharacterAnimationComponent->ForcePlayAnimMontage(PlayData);
		CharacterAnimationComponent->OnCharacterMontageEnded().AddUniqueDynamic(this, &ThisClass::HandleMontageEnded);
	}
	
	if(bShouldPlayerLockOnToNearestTarget && CurrentInstigator->IsPlayerControlled())
	{
		if(ULockOnComponent* LockOnComponent = CurrentInstigator->FindComponentByClass<ULockOnComponent>())
		{
			LockOnComponent->InterpToBestTargetForMeleeAttack();
		}
	}
}

void UMontageTrigger::Internal_IncrementComboCounter()
{
	if(++ComboSectionIncrement > MaxComboSections)
	{
		Internal_ResetComboCounter();
	}
}

void UMontageTrigger::Internal_ResetComboCounter()
{
	ComboSectionIncrement = 1;
}
