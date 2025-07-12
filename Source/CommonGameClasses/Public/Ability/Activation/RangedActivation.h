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
	TArray<FHitResult> WeaponTrace(bool bLineTrace, float CircleRadius = 5.f, bool bVisibilityTrace = false, FVector StartOverride = FVector::ZeroVector, FVector EndOverride = FVector::ZeroVector, const TArray<AActor*>& AddIgnoreActors = TArray<AActor*>());
	TArray<AActor*> GetActorsToIgnoreCollision() const;
	FHitResult AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact);

	/**
	 * An array of actor classes defining the effects to apply upon activation.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> ActivationEffectsToApply;
	/**
	 * Represents the name of the socket on the mesh component used for specific operations such as determining
	 * raycast origin or rotation.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	FName MeshSocketName;
	/**
	 * An array of affiliation types that should be ignored during activation-related processes.
	 * This can be used to customize behavior, such as excluding certain actor groups (e.g., Allies)
	 * from targeting or interactions.
	 *
	 * Default Value:
	 * - Initialized to ignore Allies (`EAffiliation::Allies`) by default.
	 */
	UPROPERTY(EditDefaultsOnly, DisplayName="Ignore Targets", Category="COMMON|Activation")
	TArray<EAffiliation> IgnoreSpecificAffiliations = { EAffiliation::Allies };

	/**
	 * Determines the direction in which activation traces should be performed.
	 * This direction influences how the ability's activation behavior, such as targeting or raycasting, is resolved.
	 *
	 * Default Value:
	 * - Initialized to `ELineTraceDirection::Camera` by default, meaning traces will be performed from the player's camera.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", DisplayName="Trace Direction")
	ELineTraceDirection ActivationTraceDirection = ELineTraceDirection::Camera;
	/**
	 * Defines the trace direction specifically for AI when performing activation traces.
	 * Overrides the default trace direction specified by `ActivationTraceDirection`
	 * when certain conditions are met, allowing for tailored behavior in AI abilities.
	 *
	 * Default Value:
	 * - Initialized to `ELineTraceDirection::AbilityMeshForwardVector`, ensuring
	 *   that AI traces default to the forward direction of the ability mesh.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", DisplayName="AI Trace Direction", meta=(EditCondition= "ActivationTraceDirection == ELineTraceDirection::InputDirection || ActivationTraceDirection == ELineTraceDirection::Camera", EditConditionHides))
	ELineTraceDirection AIActivationTraceDirectionOverride = ELineTraceDirection::AbilityMeshForwardVector;
	/**
	 * Specifies the maximum distance for activation traces in world units.
	 * Determines how far the trace will extend to detect or interact with targets,
	 * influencing the effective range of the activation ability.
	 *
	 * Default Value:
	 * - Initialized to 1000.0, providing a reasonable default range for standard interactions.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(ClampMin = 1.f))
	float TraceRange = 1000.f;
	/**
	 * Indicates if line tracing should be performed during the activation process.
	 * Line traces are typically used for target detection or interaction checks.
	 * 
	 * Default Value:
	 * - Initialized to true, meaning line tracing is enabled by default.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(ClampMin = 1.f, InlineEditConditionToggle))
	bool bShouldLineTrace = true;
	/**
	 * Determines whether the trace operation should stop after registering the first successful hit.
	 * When enabled, the trace halts upon detecting the initial valid interaction or collision.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace")
	bool bShouldStopTraceAfterFirstSuccessfulHit = true;
	/**
	 * Specifies the radius for sphere trace operations, which are used for detecting or interacting with objects
	 * during activation. The sphere trace provides a volumetric sweep instead of a line, helping detect objects
	 * within the specified radius.
	 *
	 * Default Value:
	 * - Initialized to 20.0, providing a small but reasonable default radius for the sphere trace.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(ClampMin = 1.f, EditCondition="!bShouldLineTrace"))
	float SphereTraceRadius = 20.f;

	/**
	 * Determines if firing spread should be applied when traces or projectiles are activated.
	 * Enables or disables the mechanic where shots or traces deviate from a central trajectory,
	 * introducing variability in their direction and simulating a spread effect.
	 *
	 * Default Value:
	 * - Initialized to false, meaning firing spread is disabled by default.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace", meta=(InlineEditConditionToggle))
	bool bHasFiringSpread = false;
	/**
	 * Defines the spread angle, in degrees, for the trace logic when activated.
	 * Used to simulate inaccuracy or dispersion in firing mechanics. The value will
	 * only be editable if the activation condition `bHasFiringSpread` is true.
	 * A minimum value of 0 is enforced.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float TraceSpread = 5.f;
	/**
	 * Determines the increment applied to the firing spread for each consecutive shot.
	 * Configurable only when firing spread is enabled. Must be a non-negative value.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float FiringSpreadIncrement = 1.0f;
	/**
	 * The maximum spread angle for firing in degrees. Controls the maximum possible deviation
	 * for projectiles or traces when firing. Can be edited only if firing spread is enabled.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Trace|Spread", meta = (ClampMin="0", EditCondition = "bHasFiringSpread"))
	float FiringSpreadMax = 10.f;

	/**
	 * Determines whether aim assist functionality is enabled.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trace|AimAssist", meta=(InlineEditConditionToggle))
	bool bEnableAimAssist = true;
	/**
	 * An array of object type queries used to filter objects for aim assist tracing.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trace|AimAssist", meta=(EditCondition = "bEnableAimAssist"))
	TArray<TEnumAsByte<EObjectTypeQuery>> AimAssistTraceForObjectTypes;
	/**
	 * The maximum distance within which the aim assist functionality is effective.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trace|AimAssist", meta=(EditCondition = "bEnableAimAssist"))
	float AimAssistRange = TraceRange;
	/**
	 * The angular range (in degrees) within which aim assist is activated, provided aim assist is enabled.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trace|AimAssist", meta=(EditCondition = "bEnableAimAssist"))
	float AimAssistAngle = 15.0f;
	/**
	 * Determines the strength of the aim assist functionality. A higher value increases the intensity of the assist.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trace|AimAssist", meta=(EditCondition = "bEnableAimAssist"))
	float AimAssistStrength = 0.5f;
	/**
	 * Specifies the affiliation type of actors that will be affected by aim assist.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trace|AimAssist", meta=(EditCondition = "bEnableAimAssist"))
	EAffiliation AimAssistAffiliation = EAffiliation::Enemies;
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Debug", meta=(InlineEditConditionToggle))
	bool bDrawDebugTrace = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Debug", meta=(ClampMin = 0.f, EditCondition="bDrawDebugTrace"))
	float DebugTraceDuration = 1.f;

protected:
	void GetTraceLocations(FVector& StartTrace, FVector& EndTrace);
	
private:
	FVector TryGetAimAssistDirection(const FVector& OriginalAimDirection) const;
	AActor* FindBestAimAssistTarget(const FVector& StartLocation, const FVector& AimDirection) const;
	bool IsValidAimAssistTarget(AActor* Target) const;
	
	void Internal_AssignOwningController();

	FVector Internal_GetStartTraceLocation(const FVector AimDirection) const;
	FVector Internal_GetCameraStartLocation(const FVector AimDirection) const;
	FVector Internal_GetAimDirection(ELineTraceDirection LineTraceDirection) const;
	FVector Internal_GetFiringSpreadDirection(const FVector AimDirection);
	FVector Internal_GetMouseAim() const;
	TArray<FHitResult> RemoveDuplicateHitResults(const TArray<FHitResult>& HitResults);

	UPROPERTY(Transient)
	float CurrentFiringSpread;
	UPROPERTY()
	TWeakObjectPtr<AAIController> OwningAIController;
	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwningPlayerController;
};
