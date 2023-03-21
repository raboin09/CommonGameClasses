// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/EventDeclarations.h"
#include "UObject/Interface.h"
#include "ActivationMechanism.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActivationMechanism : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IActivationMechanism
{
	GENERATED_BODY()

public:
	// Activation level is for things like charge-up weapons, combos, etc
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) PURE_VIRTUAL(IActivationMechanism::Activate,)
	virtual void Deactivate() PURE_VIRTUAL(IActivationMechanism::Deactivate,)
	virtual void SetAbilityMesh(UMeshComponent* InMeshComponent) PURE_VIRTUAL(IActivationMechanism::SetAbilityMesh, )
	
	FORCEINLINE FAbilityActivationEvent& OnActivation() { return AbilityActivationEvent; }
	FORCEINLINE FAbilityDeactivationEvent& OnDeactivation() { return AbilityDeactivationEvent; }

protected:
	FAbilityActivationEvent AbilityActivationEvent;
	FAbilityDeactivationEvent AbilityDeactivationEvent;
};
