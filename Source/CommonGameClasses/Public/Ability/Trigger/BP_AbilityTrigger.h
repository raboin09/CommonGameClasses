// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComplexTriggerBase.h"
#include "API/Ability/TriggerMechanism.h"
#include "BP_AbilityTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UBP_AbilityTrigger : public UComplexTriggerBase
{
	GENERATED_BODY()

protected:
	FORCEINLINE virtual void HandleSuccessfulTriggerPressed() override
	{
		FTriggerEventPayload TriggerEventPayload{true, 5};
		BPN_PressTrigger(TriggerEventPayload);
	}	
	FORCEINLINE virtual void HandleTriggerReleased() override
	{
		FTriggerEventPayload TriggerEventPayload;
		BPN_ReleaseTrigger(TriggerEventPayload);
	}		
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void BPN_PressTrigger(FTriggerEventPayload& TriggerEventPayload);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void BPN_ReleaseTrigger(FTriggerEventPayload& TriggerEventPayload);
};
