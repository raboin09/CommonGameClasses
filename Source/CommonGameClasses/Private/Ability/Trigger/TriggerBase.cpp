// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Trigger/AbilityTriggerBase.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAbilityTriggerBase::PressTrigger()
{
	if(!CanExecuteTriggerEvent())
	{
		return;
	}
	HandleSuccessfulTriggerPressed();
}

void UAbilityTriggerBase::ReleaseTrigger()
{
	HandleTriggerReleased();
}

bool UAbilityTriggerBase::CanExecuteTriggerEvent() const
{
	if (UGameplayTagComponent::ActorHasAnyGameplayTags(GetOwner(), AbilityBlockedTags))
	{
		return false;
	}

	if (!UGameplayTagComponent::ActorHasAllGameplayTags(GetOwner(), AbilityRequiredTags))
	{
		return false;
	}
	
	if (UGameplayTagComponent::ActorHasAnyGameplayTags(GetInstigator(), InstigatorBlockedTags))
	{
		return false;
	}

	if (!UGameplayTagComponent::ActorHasAllGameplayTags(GetInstigator(), InstigatorRequiredTags))
	{
		return false;
	}
	return true;
}
