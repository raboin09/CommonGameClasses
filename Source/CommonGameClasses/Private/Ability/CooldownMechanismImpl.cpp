#include "Ability/CooldownMechanismImpl.h"

void UCooldownMechanismImpl::OverrideCooldownTime(const float NewCooldownTime)
{
	CooldownDuration = NewCooldownTime;
}

void UCooldownMechanismImpl::SetCooldownTime(const float CooldownAdjustment)
{
	CooldownDuration = CooldownAdjustment;
}

void UCooldownMechanismImpl::MultiplyCooldownTime(const float CooldownAdjustment)
{
	CooldownDuration *= CooldownAdjustment;
}

void UCooldownMechanismImpl::StartCooldownTimer()
{
	CurrentRunningCooldownTime = CooldownDuration;
	GetWorld()->GetTimerManager().SetTimer(Timer_CooldownTick, this, &UCooldownMechanismImpl::Internal_CooldownTick, COOLDOWN_TICK_RATE, false);
	CooldownStartedEvent.Broadcast(FCooldownStartedEventPayload());
}

void UCooldownMechanismImpl::StopCooldownTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer_CooldownTick);
	CooldownEndedEvent.Broadcast(FCooldownEndedEventPayload());
}

void UCooldownMechanismImpl::PauseCooldownTimer()
{
	GetWorld()->GetTimerManager().PauseTimer(Timer_CooldownTick);
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
	TickPayload.TotalCooldown = CooldownDuration;
	CooldownTickedEvent.Broadcast(TickPayload);
	GetWorld()->GetTimerManager().SetTimer(Timer_CooldownTick, this, &UCooldownMechanismImpl::Internal_CooldownTick, COOLDOWN_TICK_RATE, false);
}