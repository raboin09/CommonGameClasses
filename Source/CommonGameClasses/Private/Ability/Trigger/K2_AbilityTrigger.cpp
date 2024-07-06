// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Trigger/K2_AbilityTrigger.h"

void UK2_AbilityTrigger::K2N_PressTrigger_Implementation(FTriggerEventPayload& TriggerEventPayload)
{
	K2_HandleBeforePressedTrigger();
	K2_HandleAfterPressedTrigger();
	TriggerPressedEvent.Broadcast(TriggerEventPayload);
}

void UK2_AbilityTrigger::K2N_ReleaseTrigger_Implementation(FTriggerEventPayload& TriggerEventPayload)
{
	TriggerReleasedEvent.Broadcast(TriggerEventPayload);
	K2_HandleReleasedTrigger();
}
