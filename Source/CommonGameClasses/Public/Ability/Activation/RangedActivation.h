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
	//~ Begin IActivationMechanism Interface
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual void Deactivate() override;
	virtual void InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent) override;
	FORCEINLINE virtual float GetOutlineRange() const override { return TraceRange; }
	//~ End IActivationMechanism Interface

	//~ Begin UObject Interface
	virtual void PostInitProperties() override;
	//~ End UObject Interface

protected:
	virtual void Fire(const FTriggerEventPayload& TriggerEventPayload) PURE_VIRTUAL(URangedActivation::Fire,)

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_PlayFireFX(const FVector& SpawnPoint);
	
	FVector GetRaycastOriginRotation() const;
	FVector GetRaycastOriginLocation() const;
	TArray<FHitResult> WeaponTrace(bool bLineTrace, float CircleRadius = 5.f, FVector StartOverride = FVector::ZeroVector, FVector EndOverride = FVector::ZeroVector);
	TArray<AActor*> GetActorsToIgnoreCollision() const;
	FHitResult AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact);
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> ActivationEffectsToApply;	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(EditCondition="DefaultLineTraceDirection == ELineTraceDirection::Mouse", EditConditionHides))
	bool bSnapCharacterRotationToEndTrace = false;
	// Socket where the muzzle or hand is
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	FName MeshSocketName;

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceForObjectTypes;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace")
	ELineTraceDirection DefaultLineTraceDirection = ELineTraceDirection::Camera;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(EditCondition="DefaultLineTraceDirection == ELineTraceDirection::Mouse", EditConditionHides))
	ELineTraceDirection GamepadLineTraceDirection = ELineTraceDirection::InstigatorForwardVector;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(ClampMin = 1.f))
	float TraceRange = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(ClampMin = 1.f, InlineEditConditionToggle))
	bool bShouldLineTrace = true;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace")
	bool bShouldStopTraceAfterFirstSuccessfulHit = true;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(ClampMin = 1.f, EditCondition="!bShouldLineTrace"))
	float SphereTraceRadius = 20.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(InlineEditConditionToggle))
	bool bHasFiringSpread = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float TraceSpread = 5.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float FiringSpreadIncrement = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float FiringSpreadMax = 10.f;

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Debug")
	bool bDrawDebugTrace = false;

	// Aim assist settings
	UPROPERTY(EditAnywhere, Category = "COMMON|AimAssist", meta = (InlineEditConditionToggle))
	bool bEnableAimAssist = true;
	UPROPERTY(EditAnywhere, Category = "COMMON|AimAssist", meta = (EditCondition = "bEnableAimAssist"))
	float AimAssistRange = 1000.f;
	UPROPERTY(EditAnywhere, Category = "COMMON|AimAssist", meta = (EditCondition = "bEnableAimAssist"))
	float AimAssistAngle = 15.0f;
	UPROPERTY(EditAnywhere, Category = "COMMON|AimAssist", meta = (EditCondition = "bEnableAimAssist"))
	float AimAssistStrength = 0.5f;
	UPROPERTY(EditAnywhere, Category = "COMMON|AimAssist", meta = (EditCondition = "bEnableAimAssist"))
	EAffiliation AimAssistAffiliation = EAffiliation::Enemies;

private:
	FVector TryGetAimAssistDirection(const FVector& OriginalAimDirection) const;
	AActor* FindBestAimAssistTarget(const FVector& StartLocation, const FVector& AimDirection) const;
	bool IsValidAimAssistTarget(AActor* Target) const;
	
	void Internal_AssignOwningController();
	void Internal_GetTraceLocations(FVector& StartTrace, FVector& EndTrace);

	FVector Internal_GetStartTraceLocation(const FVector AimDirection) const;
	FVector Internal_GetCameraStartLocation(const FVector AimDirection) const;
	FVector Internal_GetAimDirection(ELineTraceDirection LineTraceDirection) const;
	FVector Internal_GetFiringSpreadDirection(const FVector AimDirection);
	FVector Internal_GetMouseAim() const;
	static TArray<FHitResult> RemoveDuplicateHitResults(const TArray<FHitResult>& HitResults);

	UPROPERTY(Transient)
	float CurrentFiringSpread;
	UPROPERTY()
	TWeakObjectPtr<AAIController> OwningAIController;
	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwningPlayerController;

public:
	FORCEINLINE bool IsPlayerControlled() const { return OwningPlayerController != nullptr; };
};
