// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivation.h"
#include "Types/CommonEventDeclarations.h"
#include "K2_AbilityActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UK2_AbilityActivation : public UBaseActivation
{
	GENERATED_BODY()

protected:	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_Activate(const FTriggerEventPayload& TriggerEventPayload);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_Deactivate();

public:
	FORCEINLINE virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override { K2_Activate(TriggerEventPayload); }	
	FORCEINLINE virtual void Deactivate() override { K2_Deactivate(); }
};
