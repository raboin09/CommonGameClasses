// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Trigger/BP_AbilityTrigger.h"

void UBP_AbilityTrigger::BPN_PressTrigger_Implementation(FTriggerEventPayload& TriggerEventPayload)
{
	BPI_HandleBeforePressedTrigger();
	BPI_HandleAfterPressedTrigger();
	TriggerPressedEvent.Broadcast(TriggerEventPayload);
}

void UBP_AbilityTrigger::BPN_ReleaseTrigger_Implementation(FTriggerEventPayload& TriggerEventPayload)
{
	TriggerReleasedEvent.Broadcast(TriggerEventPayload);
	BPI_HandleReleasedTrigger();
}
