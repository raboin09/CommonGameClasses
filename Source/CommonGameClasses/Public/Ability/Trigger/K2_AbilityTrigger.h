// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/TriggerMechanism.h"
#include "K2_AbilityTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API AK2_AbilityTrigger : public ACommonActor, public ITriggerMechanism
{
	GENERATED_BODY()

protected:
	FORCEINLINE virtual void PressTrigger() override
	{
		const FTriggerEventPayload TriggerEventPayload{true, 5};
		K2_PressTrigger(TriggerEventPayload);
	}	
	FORCEINLINE virtual void ReleaseTrigger() override
	{
		const FTriggerEventPayload TriggerEventPayload;
		K2_ReleaseTrigger(TriggerEventPayload);
	}	
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return K2_ShouldRetriggerAbilityAfterCooldown(); }
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_PressTrigger(const FTriggerEventPayload& TriggerEventPayload);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_ReleaseTrigger(const FTriggerEventPayload& TriggerEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	bool K2_ShouldRetriggerAbilityAfterCooldown() const;
};
