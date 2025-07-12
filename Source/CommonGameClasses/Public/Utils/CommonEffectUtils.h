// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonEffectUtils.generated.h"

struct FGameplayTag;
/**
 * A utility class that provides commonly used methods for applying and managing gameplay effects.
 *
 * This class is intended to simplify the interaction with effect systems in gameplay logic.
 * It provides reusable static utility methods to safely apply, remove, or manage effects across actors.
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonEffectUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Applies a set of effects to all valid actors within a specified radius from a given origin.
	 *
	 * @param InstigatingActor The actor responsible for initiating the effects.
	 * @param EffectsToApply A list of effect classes to apply to valid targets.
	 * @param TraceOrigin The center of the radius where the effects will be applied.
	 * @param TraceRadius The radius within which the effects will be applied.
	 * @param bIgnoreAffiliation If true, effects will be applied regardless of affiliation (e.g., allies or enemies).
	 * @param bRequiresVisibilityValidation If true, checks for line-of-sight visibility between the origin and target actors.
	 * @param CollisionChannel The collision channel used for tracing to validate visibility.
	 * @param TargetCharacterValidationBone The named bone (e.g., "spine_01") on target characters to use for trace validation.
	 * @param bOverrideValidationStartLocation If true, overrides the start location for visibility trace validation.
	 * @param VisibilityTraceStartOverride The location to start visibility tracing from, if override is enabled.
	 */
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils", meta=(AdvancedDisplay="TargetCharacterValidationBone,bOverrideValidationStartLocation,VisibilityTraceStartOverride,CollisionChannel"))
	static void ApplyEffectsInRadius(
		AActor* InstigatingActor,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect")) TArray<TSubclassOf<AActor>> EffectsToApply,
		FVector TraceOrigin,
		float TraceRadius,
		bool bIgnoreAffiliation,
		bool bRequiresVisibilityValidation = true,
		ECollisionChannel CollisionChannel = ECC_Visibility,
		FName TargetCharacterValidationBone = "spine_01",
		bool bOverrideValidationStartLocation = false,
		FVector VisibilityTraceStartOverride = FVector::ZeroVector
		);
	
	/**
	 * Applies a specified effect at a given location in the world using the instigating actor as the context.
	 * Optionally activates the effect immediately upon application.
	 *
	 * @param InstigatingActor The actor initiating the effect. Must not be null.
	 * @param EffectToApply The class of the effect to be applied. Must be a valid subclass of AActor.
	 * @param Location The location in the world where the effect should be applied.
	 * @param bActivateImmediately If true, the effect will be activated immediately after being applied.
	 */
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void ApplyEffectAtLocation(
		FVector Location,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect", AllowAbstract = false)) TSubclassOf<AActor> EffectToApply,
		AActor* InstigatingActor,
		bool bActivateImmediately = true
		);
	
	/**
	 * Applies the specified effect to the given actor, if the actor has a valid effect container component.
	 *
	 * If the receiving actor or the effect to apply is null, this method will return without performing any action.
	 * The effect is applied using the actor's `UEffectContainerComponent`, if available.
	 *
	 * @param ReceivingActor The actor that will receive the effect. Must have a `UEffectContainerComponent` for the effect to be applied.
	 * @param EffectToApply The class of the effect to apply to the receiving actor.
	 */
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void ApplyEffectToActor(
		AActor* ReceivingActor,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect", AllowAbstract = false)) TSubclassOf<AActor> EffectToApply
		);
	
	/**
	 * Removes all gameplay effects from the specified actor that are associated with a given gameplay tag.
	 *
	 * @param ReceivingActor The actor from which the tagged effects should be removed. If the actor does not have an associated EffectContainerComponent, this method does nothing.
	 * @param RemoveEffectsWithTag The gameplay tag that identifies the effects to be removed from the actor.
	 */
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void RemoveTaggedEffectsFromActor(AActor* ReceivingActor, const FGameplayTag& RemoveEffectsWithTag);

	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void RemoveEffectsFromActor(
		AActor* ReceivingActor,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect", AllowAbstract = false)) TArray<TSubclassOf<AActor>> EffectClassesToRemove
		);
	
	/**
	 * Removes all effects of the specified class from the provided actor if the actor has an associated effect container component.
	 *
	 * @param ReceivingActor The actor from which effects will be removed. Must not be null.
	 * @param EffectClassToRemove The class of effects to be removed from the actor.
	 */
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void RemoveEffectFromActor(
		AActor* ReceivingActor,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect", AllowAbstract = false)) TSubclassOf<AActor> EffectClassToRemove
		);
	
	/**
	 * Applies multiple effects to a specified actor.
	 * Iterates through the provided list of effect classes and applies each effect to the receiving actor.
	 *
	 * @param EffectsToApply An array of effect classes to be applied to the receiving actor.
	 * @param ReceivingActor The actor that will receive the specified effects. This must be a valid actor.
	 */
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void ApplyEffectsToActor(
		AActor* ReceivingActor,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect", AllowAbstract = false)) TArray<TSubclassOf<AActor>> EffectsToApply
		);
	
	/**
	 * Applies a series of effects to the specified impact point described by a hit result.
	 * Each effect from the provided array of effect classes will be applied to the hit location.
	 *
	 * @param EffectsToApply Array of effect classes to be applied at the hit result location.
	 * @param Impact The hit result containing information about the impact location, normals, and target.
	 * @param InstigatingActor The actor responsible for applying these effects, often the instigator of the attack or action.
	 * @param bShouldRotateHitResult Whether the hit result should be rotated to align with the actor's orientation.
	 */
	static void ApplyEffectsToHitResult(
		const FHitResult& Impact,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect", AllowAbstract = false)) TArray<TSubclassOf<AActor>> EffectsToApply,
		AActor* InstigatingActor,
		bool bShouldRotateHitResult = true
		);
	
	/**
	 * Applies a specified effect to a target based on a hit result.
	 *
	 * This method uses information from a hit result to apply an effect defined by the specified class
	 * to the actor impacted at the hit result. The behavior can be customized by rotating the hit result
	 * according to the specified boolean flag.
	 *
	 * @param BaseEffectClass The class of the effect to apply. Must be a subclass of AActor.
	 * @param Impact The hit result that provides details about the hit, including the impacted actor.
	 * @param InstigatingActor The actor responsible for applying the effect. This is generally the actor that initiated the action.
	 * @param bShouldRotateHitResult A boolean flag indicating whether the hit result should be rotated for applying the effect.
	 */
	static void ApplyEffectToHitResult(
		const FHitResult& Impact,
		UPARAM(meta=(MustImplement="/Script/CommonGameClasses.Effect", AllowAbstract = false)) TSubclassOf<AActor> BaseEffectClass,
		AActor* InstigatingActor,
		bool bShouldRotateHitResult = true
		);
};
