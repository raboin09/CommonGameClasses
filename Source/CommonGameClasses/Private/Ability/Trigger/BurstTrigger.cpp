#include "Ability/Trigger/BurstTrigger.h"

void UBurstTrigger::HandleSuccessfulTriggerPressed()
{
	if(!bHasBurstDelayBetweenShots)
	{
		for(int ActivationCount = 0; ActivationCount < NumberOfActivations; ++ActivationCount)
		{
			Internal_BurstFireTick();
		}
	} else
	{
		if(GetOwnerWorld())
		{
			GetOwnerWorld()->GetTimerManager().SetTimer(TimerHandle_BurstFire, this, &ThisClass::Internal_BurstFireTick, TimeBetweenBurstShots, true, 0.f);	
		}
	}
}

void UBurstTrigger::HandleTriggerReleased()
{
	FTriggerEventPayload ReleaseTriggerEventPayload;
	ReleaseTriggerEventPayload.SetActivationCount(0);
	ReleaseTriggerEventPayload.bStartActivationImmediately = false;
	TriggerReleasedEvent.Broadcast(ReleaseTriggerEventPayload);
}

void UBurstTrigger::Internal_BurstFireTick()
{
	if(++BurstFireCount > NumberOfActivations)
	{
		BurstFireCount = 0;
		if(GetOwnerWorld())
		{
			GetOwnerWorld()->GetTimerManager().ClearTimer(TimerHandle_BurstFire);	
		}
		return;
	}
	FTriggerEventPayload PressTriggerEventPayload;
	PressTriggerEventPayload.SetActivationCount(BurstFireCount);
	PressTriggerEventPayload.bStartActivationImmediately = true;
	TriggerPressedEvent.Broadcast(PressTriggerEventPayload);
}
