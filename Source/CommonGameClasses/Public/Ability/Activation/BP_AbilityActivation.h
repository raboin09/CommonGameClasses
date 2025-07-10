// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivation.h"
#include "Types/CommonEventDeclarations.h"
#include "BP_AbilityActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UBP_AbilityActivation : public UBaseActivation
{
	GENERATED_BODY()

protected:	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void BPN_Activate(const FTriggerEventPayload& TriggerEventPayload);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void BPN_Deactivate();
	
public:
	FORCEINLINE virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override { Super::Activate(TriggerEventPayload); BPN_Activate(TriggerEventPayload); }	
	FORCEINLINE virtual void Deactivate() override { Super::Deactivate(); BPN_Deactivate(); }
	FORCEINLINE virtual float GetOutlineRange() const override { return DEFAULT_OUTLINE_DISTANCE; }
};
