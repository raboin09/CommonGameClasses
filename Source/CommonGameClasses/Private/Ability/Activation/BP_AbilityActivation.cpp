// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Activation/BP_AbilityActivation.h"

void UBP_AbilityActivation::BPN_Activate_Implementation(const FTriggerEventPayload& TriggerEventPayload)
{
	AbilityActivationEvent.Broadcast(FAbilityActivationEventPayload());
}

void UBP_AbilityActivation::BPN_Deactivate_Implementation()
{
	AbilityDeactivationEvent.Broadcast(FAbilityDeactivationEventPayload());
}
