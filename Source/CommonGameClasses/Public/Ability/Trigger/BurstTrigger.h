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
	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", meta=(ClampMin = 1))
	int32 NumberOfActivations = 1;
	
	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", meta=(InlineEditConditionToggle))
	bool bHasBurstDelayBetweenShots = false; 
	// The delay (if any) before activating again (e.g. a shotgun would be 0 as all the pellets fire instantaneously, a 3-round burst rifle would be
	// something like .1)
	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", meta=(EditCondition = "bHasBurstDelayBetweenShots && NumberOfActivations > 1", ClampMin = 0.1f))
	float TimeBetweenBurstShots = .1f;

protected:	
	//~ Begin UAbilityTriggerBase Interface
	virtual void HandleTriggerReleased() override;
	virtual void HandleSuccessfulTriggerPressed() override;
	//~ End UAbilityTriggerBase Interface
	
private:
	UFUNCTION()
	void Internal_BurstFireTick();

	int32 BurstFireCount;
	FTimerHandle TimerHandle_BurstFire;
};
