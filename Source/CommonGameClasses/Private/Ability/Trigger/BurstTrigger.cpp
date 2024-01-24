#include "Ability/Trigger/BurstTrigger.h"

void UBurstTrigger::PressTrigger()
{
	bTriggerHeld = true;
	if(TimeBetweenBurstShots <= 0.f)
	{
		for(int ActivationCount = 0; ActivationCount < NumberOfActivations; ++ActivationCount)
		{
			Internal_BurstFireTick();
		}
	} else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_BurstFire, this, &ThisClass::Internal_BurstFireTick, TimeBetweenBurstShots, true, 0.f);	
	}
}

void UBurstTrigger::ReleaseTrigger()
{
	bTriggerHeld = false;
	FTriggerEventPayload ReleaseTriggerEventPayload;
	ReleaseTriggerEventPayload.ActivationLevel = 0;
	ReleaseTriggerEventPayload.bStartActivationImmediately = false;
	TriggerReleasedEvent.Broadcast(ReleaseTriggerEventPayload);
}

void UBurstTrigger::Internal_BurstFireTick()
{
	if(++BurstFireCount > NumberOfActivations)
	{
		BurstFireCount = 0;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_BurstFire);
		return;
	}
	FTriggerEventPayload PressTriggerEventPayload;
	PressTriggerEventPayload.ActivationLevel = BurstFireCount;
	PressTriggerEventPayload.bStartActivationImmediately = true;
	TriggerPressedEvent.Broadcast(PressTriggerEventPayload);
}
