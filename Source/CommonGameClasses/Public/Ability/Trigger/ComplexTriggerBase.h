// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTriggerBase.h"
#include "UObject/Object.h"
#include "ComplexTriggerBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType)
class COMMONGAMECLASSES_API UComplexTriggerBase : public UAbilityTriggerBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleInitTrigger();
	// Run this just before the TriggerPress event is broadcast
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleBeforePressedTrigger();
	// Run this right after the TriggerPress event is broadcast
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleAfterPressedTrigger();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleReleasedTrigger();

	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Trigger")
	int32 K2N_GetActivationLevel() const;
	virtual int32 K2N_GetActivationLevel_Implementation() const;
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Trigger")
	FGameplayTag K2N_GetActivationDescriptorTag() const;
	virtual FGameplayTag K2N_GetActivationDescriptorTag_Implementation() const;
};
