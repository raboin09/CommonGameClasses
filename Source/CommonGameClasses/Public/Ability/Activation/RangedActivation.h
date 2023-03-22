// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Actors/CommonActor.h"
#include "API/Ability/ActivationMechanism.h"
#include "Types/AbilityTypes.h"
#include "RangedActivation.generated.h"

class UFXSystemComponent;
enum class EMeshType : uint8;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API ARangedActivation : public ACommonActor, public IActivationMechanism
{
	GENERATED_BODY()

public:
	ARangedActivation();
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual void Deactivate() override;
	FORCEINLINE virtual void SetAbilityMesh(UMeshComponent* InMeshComponent) override { MeshComponentRef = InMeshComponent; } 

protected:
	// AActor
	virtual void BeginPlay() override;

	// Ranged Activation 
	virtual void Fire(int32 ActivationLevel = -1) PURE_VIRTUAL(ARangedActivation::Fire,)

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayFireFX(const FVector& SpawnPoint);
	
	FVector GetRaycastOriginRotation() const;
	FVector GetRaycastOriginLocation() const;
	FHitResult WeaponTrace(bool bLineTrace, float CircleRadius = 5.f, FVector StartOverride = FVector::ZeroVector, FVector EndOverride = FVector::ZeroVector);
	FORCEINLINE bool ShouldLineTrace() const { return IsValid(OwningAIController); }
	TArray<AActor*> GetActorsToIgnoreCollision();
	FHitResult AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact);
	FORCEINLINE bool IsPlayerControlled() const { return OwningPlayerController != nullptr; };
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> AbilityEffects;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	ELineTraceDirection LineTraceDirection = ELineTraceDirection::Camera;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	EMeshType MeshType;
	// Socket where the muzzle or hand is
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	FName MeshSocketName;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	bool bAimOriginIsPlayerEyesInsteadOfWeapon;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	float TraceRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	bool bHasFiringSpread = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread", EditConditionHides))
	float TraceSpread = 5.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread", EditConditionHides))
	float FiringSpreadIncrement = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread", EditConditionHides))
	float FiringSpreadMax = 10.f;
	
private:
	void Internal_AssignOwningController();
	void Internal_AssignOwningMesh();
	
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
	UPROPERTY()
	UMeshComponent* MeshComponentRef;
};
