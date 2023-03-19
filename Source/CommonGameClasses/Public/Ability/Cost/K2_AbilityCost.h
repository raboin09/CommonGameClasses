// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/CostMechanism.h"
#include "K2_AbilityCost.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API AK2_AbilityCost : public ACommonActor, public ICostMechanism
{
	GENERATED_BODY()
	
protected:
	FORCEINLINE virtual bool CanConsumeResource() override { return K2_CanConsumeResource(); }
	FORCEINLINE virtual bool TryReserveResource() override { return K2_TryReserveResource(); }
	FORCEINLINE virtual void ConsumeResource() override { K2_ConsumeResource(); }
	FORCEINLINE virtual void InitCostMechanism(APawn* InCharOwner) override { K2_InitCostMechanism(); }

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	bool K2_CanConsumeResource();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	bool K2_TryReserveResource();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_ConsumeResource();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_InitCostMechanism();
};
