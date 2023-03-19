// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/EventDeclarations.h"
#include "UObject/Interface.h"
#include "CooldownMechanism.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCooldownMechanism : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API ICooldownMechanism
{
	GENERATED_BODY()

public:	
	virtual void OverrideCooldownTime(const float NewCooldownTime) = 0;
	virtual void SetCooldownTime(const float CooldownAdjustment) = 0;
	virtual void MultiplyCooldownTime(const float CooldownAdjustment) = 0;
	
	virtual void StartCooldownTimer() = 0;
	virtual void StopCooldownTimer() = 0;
	virtual void PauseCooldownTimer() = 0;
	
	DECLARE_EVENT_OneParam(ICooldownMechanism, FCooldownTickedEvent, const FCooldownTickedEventPayload&)
	FORCEINLINE FCooldownTickedEvent& OnCooldownTicked() { return CooldownTickedEvent; }
	DECLARE_EVENT_OneParam(ICooldownMechanism, FCooldownStartedEvent, const FCooldownStartedEventPayload&)
	FORCEINLINE FCooldownStartedEvent& OnCooldownTimerStarted() { return CooldownStartedEvent; }
	DECLARE_EVENT_OneParam(ICooldownMechanism, FCooldownEndedEvent, const FCooldownEndedEventPayload&)
	FORCEINLINE FCooldownEndedEvent& OnCooldownTimerEnded() { return CooldownEndedEvent; }

protected:
	FCooldownTickedEvent CooldownTickedEvent;
	FCooldownStartedEvent CooldownStartedEvent;
	FCooldownEndedEvent CooldownEndedEvent;
};

