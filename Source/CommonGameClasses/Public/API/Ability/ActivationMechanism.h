// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
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
	virtual void InitActivationMechanism() PURE_VIRTUAL(IActivationMechanism::InitActivationMechanism,)
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) PURE_VIRTUAL(IActivationMechanism::Activate,)
	virtual void Deactivate() PURE_VIRTUAL(IActivationMechanism::Deactivate,)
	
	FORCEINLINE FAbilityActivationEvent& OnActivation() { return AbilityActivationEvent; }
	FORCEINLINE FAbilityDeactivationEvent& OnDeactivation() { return AbilityDeactivationEvent; }
	
	FORCEINLINE virtual void SetInstigator(APawn* InPawn) { PawnInstigator = InPawn; }
	FORCEINLINE virtual APawn* GetInstigator() const { return PawnInstigator; }
	FORCEINLINE virtual void SetOwner(AActor* InActor) { AbilityOwner = InActor; }
	FORCEINLINE virtual AActor* GetOwner() const { return AbilityOwner; }
	
protected:
	AActor* AbilityOwner = nullptr;
	APawn* PawnInstigator = nullptr;

	FAbilityActivationEvent AbilityActivationEvent;
	FAbilityDeactivationEvent AbilityDeactivationEvent;
};
