#include "Ability/Trigger/MontageTrigger.h"

#include "ActorComponent/ActorAssetManagerComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/LockOnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Types/CommonTagTypes.h"

void UMontageTrigger::InitTrigger()
{
	K2_HandleInitTrigger();
	if(!MontageToPlay.IsValid())
	{
		if(!GetOwner())
		{
			return;
		}
		
		if(UActorAssetManagerComponent* ActorAssetManagerComponent = GetOwner()->FindComponentByClass<UActorAssetManagerComponent>())
		{
			FLoadedAnimMontageEvent LoadedAnimMontageEvent = FLoadedAnimMontageEvent();
			ActorAssetManagerComponent->K2_Async_LoadAnimMontageObject(MontageToPlay, true, LoadedAnimMontageEvent);
		}
	}
	
	CharacterAnimationComponent = GetInstigator()->FindComponentByClass<UCharacterAnimationComponent>();
}

void UMontageTrigger::PressTrigger()
{	
	if(UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameAbilityEvent::ComboActivated))
	{
		Internal_IncrementComboCounter();
	} else
	{
		UKismetSystemLibrary::PrintString(this, "Soft Reset");
		Internal_ResetComboCounter();
	}
	Internal_StartMontage();
}

void UMontageTrigger::ReleaseTrigger()
{
	FTriggerEventPayload ReleaseTriggerEventPayload;
	ReleaseTriggerEventPayload.ActivationLevel = 0;
	ReleaseTriggerEventPayload.bStartActivationImmediately = false;
	ReleaseTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerReleasedEvent.Broadcast(ReleaseTriggerEventPayload);
	K2_HandleReleasedTrigger();
}

void UMontageTrigger::ResetTrigger()
{
	Internal_ResetComboCounter();
	TArray<FGameplayTag> StateTagsToRemove;
	StateTagsToRemove.Add(CommonGameAbilityEvent::ComboActivated);
	StateTagsToRemove.Add(CommonGameAbilityEvent::Active);
	StateTagsToRemove.Add(CommonGameAbilityEvent::Activated);
	UGameplayTagComponent::RemoveTagsFromActor(GetOwner(), StateTagsToRemove);
}

void UMontageTrigger::HandleMontageEnded(const FCharacterMontageEndedPayload& CharacterMontageEndedPayload)
{
	if(CharacterMontageEndedPayload.EndedMontage == MontageToPlay.Get() && CharacterMontageEndedPayload.UpcomingMontage != MontageToPlay.Get())
	{
		ResetTrigger();
		CharacterAnimationComponent->OnCharacterMontageEnded().RemoveDynamic(this, &ThisClass::HandleMontageEnded);
	}
}

FAnimMontagePlayData UMontageTrigger::Internal_GetPlayData() const
{
	FAnimMontagePlayData PlayData;	
	PlayData.MontageToPlay = MontageToPlay.Get();
	PlayData.MontageSection = Internal_GetNextMontageSection();
	return PlayData;
}

FName UMontageTrigger::Internal_GetNextMontageSection() const
{
	if(bRandomizeMontages)
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
	
	if(CharacterAnimationComponent.IsValid())
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
	K2_HandlePressedTrigger();

	FTriggerEventPayload PressTriggerEventPayload;
	PressTriggerEventPayload.ActivationLevel = ComboSectionIncrement;
	// Activation waits for montage notify to start
	PressTriggerEventPayload.bStartActivationImmediately = false;
	PressTriggerEventPayload.bMontageDrivesActivation = true;
	TriggerPressedEvent.Broadcast(PressTriggerEventPayload);
}

void UMontageTrigger::Internal_IncrementComboCounter()
{
	UKismetSystemLibrary::PrintString(this, FString::FromInt(ComboSectionIncrement + 1));
	if(++ComboSectionIncrement > MaxComboSections)
	{
		Internal_ResetComboCounter();
	}
}

void UMontageTrigger::Internal_ResetComboCounter()
{
	ComboSectionIncrement = 1;
}
