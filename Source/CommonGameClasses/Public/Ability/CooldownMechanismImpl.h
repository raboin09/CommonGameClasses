// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/CooldownMechanism.h"
#include "UObject/Object.h"
#include "CooldownMechanismImpl.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class COMMONGAMECLASSES_API UCooldownMechanismImpl : public UObject, public ICooldownMechanism
{
	GENERATED_BODY()

public:
	//~ Begin ICooldownMechanism Interface
	virtual void StartCooldownTimer() override;
	virtual void StopCooldownTimer() override;
	virtual void PauseCooldownTimer() override;
	//~ End ICooldownMechanism Interface
	
	UPROPERTY(EditAnywhere, Category="COMMON|Cooldown")
	float CooldownDuration = 1.f;
	
private:
	void Internal_CooldownTick();
	
	FTimerHandle Timer_CooldownTick;
	float TotalCooldownTime;
	float CurrentRunningCooldownTime;
	const float COOLDOWN_TICK_RATE = .05f;
};
