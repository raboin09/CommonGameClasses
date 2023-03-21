#include "Ability/CooldownMechanismImpl.h"

#include "Types/CoreTypes.h"

void UCooldownMechanismImpl::StartCooldownTimer(const float CooldownDuration)
{
	TotalCooldownTime = CooldownDuration;
	CurrentRunningCooldownTime = CooldownDuration;
	GetWorld()->GetTimerManager().SetTimer(Timer_CooldownTick, this, &UCooldownMechanismImpl::Internal_CooldownTick, COOLDOWN_TICK_RATE, false);
	CooldownStartedEvent.Broadcast(FCooldownStartedEventPayload());
}

void UCooldownMechanismImpl::StopCooldownTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer_CooldownTick);
	TotalCooldownTime = 0.f;
	CurrentRunningCooldownTime = 0.f;
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
	TickPayload.TotalCooldown = TotalCooldownTime;
	CooldownTickedEvent.Broadcast(TickPayload);
	GetWorld()->GetTimerManager().SetTimer(Timer_CooldownTick, this, &UCooldownMechanismImpl::Internal_CooldownTick, COOLDOWN_TICK_RATE, false);
}