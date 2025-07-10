// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTriggerBase.h"
#include "BurstTrigger.generated.h"

UCLASS(BlueprintType, Blueprintable)
class COMMONGAMECLASSES_API UBurstTrigger : public UAbilityTriggerBase
{
	GENERATED_BODY()

public:
	// Number of times the ability will be activated after the trigger is pressed (e.g. a shotgun would have 6, a 3-round burst rifle would have 3)
	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", meta=(ClampMin = 0.f))
	int32 NumberOfActivations = 1;
	// The delay (if any) before activating again (e.g. a shotgun would be 0 as all the pellets fire instantaneously, a 3-round burst rifle would be
	// something like .1)
	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", meta=(EditCondition = "NumberOfActivations > 1", ClampMin = 0.f))
	float TimeBetweenBurstShots = .1f;
	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", meta=(ClampMin = 0.f))
	bool bShouldRetriggerAbilityAfterCooldown = false;

protected:
	//~ Begin ITriggerMechanism Interface
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return (bShouldRetriggerAbilityAfterCooldown && bTriggerHeld); }
	//~ End ITriggerMechanism Interface
	
	//~ Begin UAbilityTriggerBase Interface
	virtual void HandleTriggerReleased() override;
	virtual void HandleSuccessfulTriggerPressed() override;
	//~ End UAbilityTriggerBase Interface
	
private:
	UFUNCTION()
	void Internal_BurstFireTick();

	bool bTriggerHeld;
	int32 BurstFireCount;
	FTimerHandle TimerHandle_BurstFire;
};
