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
	void BPI_HandleInitTrigger();
	// Run this just before the TriggerPress event is broadcast
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_HandleBeforePressedTrigger();
	// Run this right after the TriggerPress event is broadcast
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_HandleAfterPressedTrigger();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_HandleReleasedTrigger();

	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Trigger")
	int32 BPN_GetActivationLevel() const;
	virtual int32 BPN_GetActivationLevel_Implementation() const;
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Trigger")
	FGameplayTag BPN_GetActivationDescriptorTag() const;
	virtual FGameplayTag BPN_GetActivationDescriptorTag_Implementation() const;
};
