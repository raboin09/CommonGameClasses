// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Trigger/K2_AbilityTrigger.h"

void AK2_AbilityTrigger::K2_PressTrigger_Implementation(const FTriggerEventPayload& TriggerEventPayload)
{
	TriggerActivatedEvent.Broadcast(TriggerEventPayload);
}

void AK2_AbilityTrigger::K2_ReleaseTrigger_Implementation(const FTriggerEventPayload& TriggerEventPayload)
{
	TriggerDeactivatedEvent.Broadcast(TriggerEventPayload); 
}
