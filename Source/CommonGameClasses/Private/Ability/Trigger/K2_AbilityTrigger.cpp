// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Trigger/K2_AbilityTrigger.h"

void AK2_AbilityTrigger::K2_PressTrigger_Implementation(float& CooldownOverride, int32& ActivationLevel)
{
	TriggerActivatedEvent.Broadcast({ActivationLevel});
}

void AK2_AbilityTrigger::K2_ReleaseTrigger_Implementation()
{
	TriggerDeactivatedEvent.Broadcast({}); 
}
