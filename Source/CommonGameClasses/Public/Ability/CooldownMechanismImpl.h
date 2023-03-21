// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/CooldownMechanism.h"
#include "UObject/Object.h"
#include "CooldownMechanismImpl.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COMMONGAMECLASSES_API UCooldownMechanismImpl : public UObject, public ICooldownMechanism
{
	GENERATED_BODY()

public:
	virtual void StartCooldownTimer(const float CooldownDuration) override;
	virtual void StopCooldownTimer() override;
	virtual void PauseCooldownTimer() override;
	
private:
	void Internal_CooldownTick();
	
	FTimerHandle Timer_CooldownTick;
	float TotalCooldownTime;
	float CurrentRunningCooldownTime;
	const float COOLDOWN_TICK_RATE = .05f;
};
