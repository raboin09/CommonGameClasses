#include "Ability/CooldownMechanismImpl.h"

void UCooldownMechanismImpl::StartCooldownTimer()
{
	TotalCooldownTime = CooldownDuration;
	CurrentRunningCooldownTime = CooldownDuration;
	if(GetOwnerWorld())
	{
		GetOwnerWorld()->GetTimerManager().SetTimer(Timer_CooldownTick, this, &ThisClass::Internal_CooldownTick, COOLDOWN_TICK_RATE, false);	
	}
	CooldownStartedEvent.Broadcast(FCooldownStartedEventPayload());
}

void UCooldownMechanismImpl::StopCooldownTimer()
{
	if(GetOwnerWorld())
	{
		GetOwnerWorld()->GetTimerManager().ClearTimer(Timer_CooldownTick);
	}
	TotalCooldownTime = 0.f;
	CurrentRunningCooldownTime = 0.f;
	CooldownEndedEvent.Broadcast(FCooldownEndedEventPayload());
}

void UCooldownMechanismImpl::PauseCooldownTimer()
{
	if(GetOwnerWorld())
	{
		GetOwnerWorld()->GetTimerManager().PauseTimer(Timer_CooldownTick);
	}
}

void UCooldownMechanismImpl::Internal_CooldownTick()
{
	CurrentRunningCooldownTime -= COOLDOWN_TICK_RATE;

	if(CurrentRunningCooldownTime <= 0)
	{
		StopCooldownTimer();
		return;
	}
	
	FCooldownTickedEventPayload TickPayload;
	TickPayload.CooldownLeft = CurrentRunningCooldownTime;
	TickPayload.TotalCooldown = TotalCooldownTime;
	CooldownTickedEvent.Broadcast(TickPayload);
	if(GetOwnerWorld())
	{
		GetOwnerWorld()->GetTimerManager().SetTimer(Timer_CooldownTick, this, &ThisClass::Internal_CooldownTick, COOLDOWN_TICK_RATE, false);
	}
}