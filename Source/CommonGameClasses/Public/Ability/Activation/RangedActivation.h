﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivation.h"
#include "Actors/CommonActor.h"
#include "Types/CommonAbilityTypes.h"
#include "RangedActivation.generated.h"

class UFXSystemComponent;
class AAIController;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API URangedActivation : public UBaseActivation
{
	GENERATED_BODY()

public:
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual void Deactivate() override;
	virtual void InitActivationMechanism() override;

protected:
	// Ranged Activation 
	virtual void Fire(int32 ActivationLevel = -1) PURE_VIRTUAL(ARangedActivation::Fire,)

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_PlayFireFX(const FVector& SpawnPoint);
	
	FVector GetRaycastOriginRotation() const;
	FVector GetRaycastOriginLocation() const;
	FHitResult WeaponTrace(bool bLineTrace, float CircleRadius = 5.f, FVector StartOverride = FVector::ZeroVector, FVector EndOverride = FVector::ZeroVector);
	TArray<AActor*> GetActorsToIgnoreCollision();
	FHitResult AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact);
	
	UPROPERTY(EditDefaultsOnly, Category="Activation", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> AbilityEffects;
	UPROPERTY(EditDefaultsOnly, Category="Activation")
	ELineTraceDirection LineTraceDirection = ELineTraceDirection::Camera;
	// Socket where the muzzle or hand is
	UPROPERTY(EditDefaultsOnly, Category="Activation")
	FName MeshSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Activation")
	bool bAimOriginIsPlayerEyesInsteadOfWeapon;
	UPROPERTY(EditDefaultsOnly, Category="Activation")
	float TraceRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="Activation")
	bool bHasFiringSpread = false;
	UPROPERTY(EditDefaultsOnly, Category="Activation|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread", EditConditionHides))
	float TraceSpread = 5.f;
	UPROPERTY(EditDefaultsOnly, Category="Activation|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread", EditConditionHides))
	float FiringSpreadIncrement = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category="Activation|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread", EditConditionHides))
	float FiringSpreadMax = 10.f;
	
private:
	void Internal_AssignOwningController();
	
	void Internal_GetTraceLocations(FVector& StartTrace, FVector& EndTrace);

	FVector Internal_GetStartTraceLocation(const FVector AimDirection) const;
	FVector Internal_GetCameraStartLocation(const FVector AimDirection) const;
	FVector Internal_GetAimDirection() const;
	FVector Internal_GetFiringSpreadDirection(const FVector AimDirection);
	FVector Internal_GetMouseAim() const;

	UPROPERTY(Transient)
	float CurrentFiringSpread;
	UPROPERTY()
	const AAIController* OwningAIController;
	UPROPERTY()
	const APlayerController* OwningPlayerController;

public:
	FORCEINLINE bool IsPlayerControlled() const { return OwningPlayerController != nullptr; };
	FORCEINLINE bool ShouldLineTrace() const { return OwningAIController != nullptr; }
};
