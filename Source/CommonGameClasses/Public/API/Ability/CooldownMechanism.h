// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/EventDeclarations.h"
#include "UObject/Interface.h"
#include "CooldownMechanism.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
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
	UFUNCTION()
	virtual void StartCooldownTimer(const float CooldownDuration) = 0;
	UFUNCTION()
	virtual void StopCooldownTimer() = 0;
	UFUNCTION()
	virtual void PauseCooldownTimer() = 0;
	
	FORCEINLINE FCooldownTickedEvent& OnCooldownTicked() { return CooldownTickedEvent; }
	FORCEINLINE FCooldownStartedEvent& OnCooldownTimerStarted() { return CooldownStartedEvent; }
	FORCEINLINE FCooldownEndedEvent& OnCooldownTimerEnded() { return CooldownEndedEvent; }

protected:
	FCooldownTickedEvent CooldownTickedEvent;
	FCooldownStartedEvent CooldownStartedEvent;
	FCooldownEndedEvent CooldownEndedEvent;
};

