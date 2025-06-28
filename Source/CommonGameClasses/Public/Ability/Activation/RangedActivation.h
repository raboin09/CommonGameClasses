// Fill out your copyright notice in the Description page of Project Settings.

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
	virtual void InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent) override;
	FORCEINLINE virtual float GetOutlineRange() const override { return TraceRange; }

protected:
	// Ranged Activation 
	virtual void Fire(const FTriggerEventPayload& TriggerEventPayload) PURE_VIRTUAL(ARangedActivation::Fire,)

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_PlayFireFX(const FVector& SpawnPoint);
	
	FVector GetRaycastOriginRotation() const;
	FVector GetRaycastOriginLocation() const;
	FHitResult WeaponTrace(bool bLineTrace, float CircleRadius = 5.f, FVector StartOverride = FVector::ZeroVector, FVector EndOverride = FVector::ZeroVector);
	TArray<AActor*> GetActorsToIgnoreCollision() const;
	FHitResult AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact);
	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> AbilityEffects;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation")
	ELineTraceDirection LineTraceDirection = ELineTraceDirection::Camera;
	// Socket where the muzzle or hand is
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation")
	FName MeshSocketName;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation", meta=(ClampMin = 0.f))
	float TraceRange = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation")
	float TraceRadius = 20.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation")
	bool bHasFiringSpread = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float TraceSpread = 5.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float FiringSpreadIncrement = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float FiringSpreadMax = 10.f;

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Activation")
	bool bDrawDebugTrace = false;
	
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
	TWeakObjectPtr<AAIController> OwningAIController;
	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwningPlayerController;

public:
	FORCEINLINE bool IsPlayerControlled() const { return OwningPlayerController != nullptr; };
	FORCEINLINE bool ShouldLineTrace() const { return OwningAIController != nullptr; }
};
