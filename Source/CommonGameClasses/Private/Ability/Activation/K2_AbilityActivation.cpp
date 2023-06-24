// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Activation/K2_AbilityActivation.h"

void AK2_AbilityActivation::K2_Activate_Implementation(const FTriggerEventPayload& TriggerEventPayload)
{
	AbilityActivationEvent.Broadcast(FAbilityActivationEventPayload());
}

void AK2_AbilityActivation::K2_Deactivate_Implementation()
{
	AbilityDeactivationEvent.Broadcast(FAbilityDeactivationEventPayload());
}
