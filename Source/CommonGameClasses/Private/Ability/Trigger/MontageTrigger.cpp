#include "Ability/Trigger/MontageTrigger.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/LockOnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Types/CommonTagTypes.h"

void AMontageTrigger::PressTrigger()
{
	if(UGameplayTagComponent::ActorHasGameplayTag(GetInstigator(), TAG_ABILITY_COMBO_WINDOW_ENABLED))
	{
		UGameplayTagComponent::AddTagToActor(GetOwner(), TAG_ABILITY_COMBO_ACTIVATED);
		UGameplayTagComponent::RemoveTagFromActor(GetOwner(), TAG_ABILITY_COMBO_WINDOW_ENABLED);
		Internal_IncrementComboCounter();
		if(!UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), TAG_ABILITY_ACTIVE))
		{
			Internal_StartMontage();
		}
	}
	else if(!UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), TAG_ABILITY_ACTIVE))
	{
		Internal_StartMontage();
	}
}

void AMontageTrigger::ReleaseTrigger()
{
	FTriggerEventPayload ReleaseTriggerEventPayload;
	ReleaseTriggerEventPayload.ActivationLevel = 0;
	ReleaseTriggerEventPayload.bStartActivationImmediately = false;
	ReleaseTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerReleasedEvent.Broadcast(ReleaseTriggerEventPayload);
}

void AMontageTrigger::ResetTrigger()
{
	Internal_ResetComboCounter();
}

FAnimMontagePlayData AMontageTrigger::Internal_GetPlayData() const
{
	FAnimMontagePlayData PlayData;	
	PlayData.MontageToPlay = MontageToPlay;
	PlayData.MontageSection = Internal_GetNextMontageSection();
	return PlayData;
}

FName AMontageTrigger::Internal_GetNextMontageSection() const
{
	if(bRandomizeMontages)
	{
		return FName(ComboPrefix + FString::FromInt(UKismetMathLibrary::RandomIntegerInRange(1, MaxComboSections)));
	}
	return FName(ComboPrefix + FString::FromInt(ComboSectionIncrement));
}

void AMontageTrigger::Internal_StartMontage()
{
	const APawn* CurrentInstigator = GetInstigator();
	if(!CurrentInstigator)
	{
		return;
	}
	
	if(UCharacterAnimationComponent* CharacterAnimationComponent = CurrentInstigator->FindComponentByClass<UCharacterAnimationComponent>())
	{
		const FAnimMontagePlayData PlayData = Internal_GetPlayData();
		CachedComboSection = PlayData.MontageSection;
		CharacterAnimationComponent->TryPlayAnimMontage(PlayData);
	}
	
	if(bShouldPlayerLockOnToNearestTarget)
	{
		if(ULockOnComponent* LockOnComponent = CurrentInstigator->FindComponentByClass<ULockOnComponent>(); LockOnComponent && CurrentInstigator->IsPlayerControlled())
		{
			LockOnComponent->InterpToBestTargetForMeleeAttack();
		}
	}
	K2_AfterPressedTrigger();

	FTriggerEventPayload PressTriggerEventPayload;
	PressTriggerEventPayload.ActivationLevel = ComboSectionIncrement;
	// Activation waits for montage notify to start
	PressTriggerEventPayload.bStartActivationImmediately = false;
	PressTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerPressedEvent.Broadcast(PressTriggerEventPayload);
}

void AMontageTrigger::Internal_IncrementComboCounter()
{
	if(++ComboSectionIncrement > MaxComboSections)
	{
		Internal_ResetComboCounter();
	}
}

void AMontageTrigger::Internal_ResetComboCounter()
{
	ComboSectionIncrement = 1;
}
