// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "Types/CommonInteractTypes.h"
#include "UObject/Interface.h"
#include "ActivationMechanism.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
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
	// Activation level is for things like charge-up weapons, combos, etc. Optional AbilityOutlineRange to adjust outline range based on weapon.
	virtual void InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent) PURE_VIRTUAL(IActivationMechanism::InitActivationMechanism,)
	
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) PURE_VIRTUAL(IActivationMechanism::Activate,)
	virtual void Deactivate() PURE_VIRTUAL(IActivationMechanism::Deactivate,)
	
protected:
	TWeakObjectPtr<AActor> AbilityOwner = nullptr;
	TWeakObjectPtr<APawn> PawnInstigator = nullptr;

	FAbilityActivationEvent AbilityActivationEvent;
	FAbilityDeactivationEvent AbilityDeactivationEvent;

public:
	FORCEINLINE virtual float GetOutlineRange() const { return INTERACT_OUTLINE_DISTANCE; }
	FORCEINLINE FAbilityActivationEvent& OnActivation() { return AbilityActivationEvent; }
	FORCEINLINE FAbilityDeactivationEvent& OnDeactivation() { return AbilityDeactivationEvent; }
	FORCEINLINE virtual void SetInstigator(TWeakObjectPtr<APawn> InPawn) { PawnInstigator = InPawn; }
	FORCEINLINE virtual void SetOwner(TWeakObjectPtr<AActor> InActor) { AbilityOwner = InActor; }
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual APawn* GetInstigator() const { return PawnInstigator.Get(); }
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual AActor* GetOwner() const { return AbilityOwner.Get(); }
};
