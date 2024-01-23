// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrigger.h"
#include "UObject/Object.h"
#include "BaseComplexTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType)
class COMMONGAMECLASSES_API UBaseComplexTrigger : public UBaseTrigger
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleInitTrigger();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandlePressedTrigger();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleReleasedTrigger();
};
