// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrigger.h"
#include "API/Ability/TriggerMechanism.h"
#include "K2_AbilityTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UK2_AbilityTrigger : public UBaseTrigger
{
	GENERATED_BODY()

protected:
	FORCEINLINE virtual void PressTrigger() override
	{
		FTriggerEventPayload TriggerEventPayload{true, 5};
		K2_PressTrigger(TriggerEventPayload);
	}	
	FORCEINLINE virtual void ReleaseTrigger() override
	{
		FTriggerEventPayload TriggerEventPayload;
		K2_ReleaseTrigger(TriggerEventPayload);
	}	
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return K2_ShouldRetriggerAbilityAfterCooldown(); }
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_PressTrigger(FTriggerEventPayload& TriggerEventPayload);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_ReleaseTrigger(FTriggerEventPayload& TriggerEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	bool K2_ShouldRetriggerAbilityAfterCooldown() const;
};
