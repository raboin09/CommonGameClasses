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
	FTriggerEventPayload ReleaseTriggerEventPayload;
	ReleaseTriggerEventPayload.ActivationLevel = 0;
	ReleaseTriggerEventPayload.bStartActivationImmediately = false;
	TriggerDeactivatedEvent.Broadcast(ReleaseTriggerEventPayload);
}

void ABurstTrigger::Internal_BurstFireTick()
{
	if(++BurstFireCount > NumberOfShotsPerFire)
	{
		BurstFireCount = 0;
		GetWorldTimerManager().ClearTimer(TimerHandle_BurstFire);
		return;
	}
	FTriggerEventPayload PressTriggerEventPayload;
	PressTriggerEventPayload.ActivationLevel = BurstFireCount;
	PressTriggerEventPayload.bStartActivationImmediately = true;
	TriggerActivatedEvent.Broadcast(PressTriggerEventPayload);
}
