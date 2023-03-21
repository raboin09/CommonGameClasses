// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Trigger/BurstTrigger.h"

ABurstTrigger::ABurstTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABurstTrigger::PressTrigger()
{
	bTriggerHeld = true;
	GetWorldTimerManager().SetTimer(TimerHandle_BurstFire, this, &ABurstTrigger::Internal_BurstFireTick, TimeBetweenBurstShots, true, 0.f);
}

void ABurstTrigger::ReleaseTrigger()
{
	bTriggerHeld = false;
	TriggerDeactivatedEvent.Broadcast(FTriggerEventPayload(0));	
}

void ABurstTrigger::Internal_BurstFireTick()
{
	if(++BurstFireCount > NumberOfShotsPerFire)
	{
		BurstFireCount = 0;
		GetWorldTimerManager().ClearTimer(TimerHandle_BurstFire);
		return;
	}
	TriggerActivatedEvent.Broadcast(FTriggerEventPayload(BurstFireCount));
}
