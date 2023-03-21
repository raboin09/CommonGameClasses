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
		int ActivationLevel = 0;
		float CooldownOverride = -1.f;
		K2_PressTrigger(CooldownOverride, ActivationLevel);
	}	
	FORCEINLINE virtual void ReleaseTrigger() override { K2_ReleaseTrigger(); }	
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return K2_ShouldRetriggerAbilityAfterCooldown(); }
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_PressTrigger(float& CooldownOverride, int32& ActivationLevel);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_ReleaseTrigger();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	bool K2_ShouldRetriggerAbilityAfterCooldown() const;

	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	FORCEINLINE void BroadcastTriggerActivated() const { }
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	FORCEINLINE void BroadcastTriggerDeactivated() const { }
};
