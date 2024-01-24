// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Activation/K2_AbilityActivation.h"

void UK2_AbilityActivation::K2N_Activate_Implementation(const FTriggerEventPayload& TriggerEventPayload)
{
	AbilityActivationEvent.Broadcast(FAbilityActivationEventPayload());
}

void UK2_AbilityActivation::K2N_Deactivate_Implementation()
{
	AbilityDeactivationEvent.Broadcast(FAbilityDeactivationEventPayload());
}
