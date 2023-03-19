// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/ActivationMechanism.h"
#include "Types/EventDeclarations.h"
#include "K2_AbilityActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API AK2_AbilityActivation : public ACommonActor, public IActivationMechanism
{
	GENERATED_BODY()

protected:
	FORCEINLINE virtual void Activate() override { K2_Activate(); }	
	FORCEINLINE virtual void Deactivate() override { K2_Deactivate(); }
	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_Activate();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_Deactivate();

	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	void BroadcastAbilityActivated() { AbilityActivationEvent.Broadcast(FAbilityActivationEventPayload()); }
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	void BroadcastAbilityDeactivated() { AbilityDeactivationEvent.Broadcast(FAbilityDeactivationEventPayload()); }
};
