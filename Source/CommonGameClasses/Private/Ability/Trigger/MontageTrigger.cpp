#include "Ability/Trigger/MontageTrigger.h"

#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/LockOnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Types/CommonTagTypes.h"

void UMontageTrigger::InitTriggerMechanism()
{
	BPI_HandleInitTrigger();
	CharacterAnimationComponent = GetInstigator()->FindComponentByClass<UCharacterAnimationComponent>();
	check(CharacterAnimationComponent.IsValid())
}

void UMontageTrigger::HandleSuccessfulTriggerPressed()
{
	if(bHasCombos)
	{
		if(UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonAbilityStateTags::ComboActivated))
		{
			Internal_IncrementComboCounter();
		} else
		{
			Internal_ResetComboCounter();
		}
	}
	BPI_HandleBeforePressedTrigger();
	Internal_StartMontage();
	BPI_HandleAfterPressedTrigger();
	FTriggerEventPayload PressTriggerEventPayload;
	PressTriggerEventPayload.AddActivationTagLevel(BPN_GetActivationDescriptorTag(), BPN_GetActivationLevel());
	// Activation waits for montage notify to start
	PressTriggerEventPayload.bStartActivationImmediately = false;
	PressTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerPressedEvent.Broadcast(PressTriggerEventPayload);
}

void UMontageTrigger::HandleTriggerReleased()
{
	FTriggerEventPayload ReleaseTriggerEventPayload;
	ReleaseTriggerEventPayload.SetActivationCount(-1);
	// We wait for the montage notify to active the weapon
	ReleaseTriggerEventPayload.bStartActivationImmediately = false;
	ReleaseTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerReleasedEvent.Broadcast(ReleaseTriggerEventPayload);
	BPI_HandleReleasedTrigger();
}

int32 UMontageTrigger::BPN_GetActivationLevel_Implementation() const
{
	return bHasCombos ? ComboSectionIncrement : 1;
}

void UMontageTrigger::ResetTrigger()
{
	TArray<FGameplayTag> StateTagsToRemove;
	StateTagsToRemove.Add(CommonAbilityStateTags::Active);
	StateTagsToRemove.Add(CommonAbilityStateTags::Activated);
	if(bHasCombos)
	{
		StateTagsToRemove.Add(CommonAbilityStateTags::ComboActivated);
		Internal_ResetComboCounter();
	}
	UGameplayTagComponent::RemoveTagsFromActor(GetOwner(), StateTagsToRemove);
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
	PlayData.MontageSection = BPN_GetNextMontageSection();
	return PlayData;
}

FName UMontageTrigger::BPN_GetNextMontageSection_Implementation() const
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

	if(bShouldPlayerLockOnToNearestTarget)
	{
		if(ULockOnComponent* LockOnComponent = CurrentInstigator->FindComponentByClass<ULockOnComponent>())
		{
			LockOnComponent->InterpToBestTargetForMeleeAttack();
		}
	}
	
	if(!CharacterAnimationComponent.IsStale())
	{
		const FAnimMontagePlayData PlayData = Internal_GetPlayData();
		CharacterAnimationComponent->ForcePlayAnimMontage(PlayData);
		CharacterAnimationComponent->OnCharacterMontageEnded().AddUniqueDynamic(this, &ThisClass::HandleMontageEnded);
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
