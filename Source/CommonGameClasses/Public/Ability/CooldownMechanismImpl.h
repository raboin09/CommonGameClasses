// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/CooldownMechanism.h"
#include "UObject/Object.h"
#include "CooldownMechanismImpl.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class COMMONGAMECLASSES_API UCooldownMechanismImpl : public UObject, public ICooldownMechanism
{
	GENERATED_BODY()

public:
	virtual void OverrideCooldownTime(const float NewCooldownTime) override;
	virtual void SetCooldownTime(const float CooldownAdjustment) override;
	virtual void MultiplyCooldownTime(const float CooldownAdjustment) override;
	
	virtual void StartCooldownTimer() override;
	virtual void StopCooldownTimer() override;
	virtual void PauseCooldownTimer() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	float CooldownDuration;

private:
	void Internal_CooldownTick();
	
	FTimerHandle Timer_CooldownTick;
	float CurrentRunningCooldownTime;
	const float COOLDOWN_TICK_RATE = .05f;
};
