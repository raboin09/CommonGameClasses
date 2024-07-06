// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrigger.h"
#include "BurstTrigger.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class COMMONGAMECLASSES_API UBurstTrigger : public UBaseTrigger
{
	GENERATED_BODY()

protected:
	virtual void HandleTriggerReleased() override;
	virtual void HandleSuccessfulTriggerPressed() override;
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return bTriggerHeld; }
	
	// Number of times the ability will be activated after the trigger is pressed (e.g. a shotgun would have 6, a 3-round burst rifle would have 3)
	UPROPERTY(EditAnywhere, Category="Trigger", meta=(ClampMin = 0.f))
	int32 NumberOfActivations = 3;
	// The delay (if any) before activating again (e.g. a shotgun would be 0 as all the pellets fire instantaneously, a 3-round burst rifle would be
	// something like .1)
	UPROPERTY(EditAnywhere, Category="Trigger", meta=(EditCondition = "NumberOfActivations > 1", ClampMin = 0.f))
	float TimeBetweenBurstShots = .1f;
	
private:	
	UFUNCTION()
	void Internal_BurstFireTick();

	bool bTriggerHeld;
	int32 BurstFireCount;
	FTimerHandle TimerHandle_BurstFire;
};
