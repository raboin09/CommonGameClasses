// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Trigger/K2_AbilityTrigger.h"

void UK2_AbilityTrigger::K2_PressTrigger_Implementation(const FTriggerEventPayload& TriggerEventPayload)
{
	TriggerPressedEvent.Broadcast(TriggerEventPayload);
}

void UK2_AbilityTrigger::K2_ReleaseTrigger_Implementation(const FTriggerEventPayload& TriggerEventPayload)
{
	TriggerReleasedEvent.Broadcast(TriggerEventPayload); 
}
