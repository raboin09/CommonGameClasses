// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonEffectUtils.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonEffectUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils", meta=(AdvancedDisplay="HitValidationBone,bOverrideValidationStartLocation,ValidationTraceStartOverride"))
	static void ApplyEffectsInRadius(AActor* InstigatingActor, TArray<TSubclassOf<AActor>> EffectsToApply, FVector TraceOrigin, float TraceRadius, ETraceTypeQuery ValidationTraceType,
		bool bIgnoreAffiliation, bool bValidateHit = true, FName HitValidationBone = "spine_01", bool bOverrideValidationStartLocation = false,
		FVector ValidationTraceStartOverride = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void ApplyEffectAtLocation(AActor* InstigatingActor, TSubclassOf<AActor> EffectToApply, FVector Location, bool bActivateImmediately = true);
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void ApplyEffectToActor(AActor* ReceivingActor, TSubclassOf<AActor> EffectToApply);
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void RemoveTaggedEffectsFromActor(AActor* ReceivingActor, const FGameplayTag& RemoveEffectsWithTag);
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void RemoveEffectsWithClassFromActor(AActor* ReceivingActor,TSubclassOf<AActor> EffectClassToRemove);
	UFUNCTION(BlueprintCallable, Category="COMMON|EffectUtils")
	static void ApplyEffectsToActor(TArray<TSubclassOf<AActor>> EffectsToApply, AActor* ReceivingActor);
	static void ApplyEffectsToHitResult(TArray<TSubclassOf<AActor>> EffectsToApply, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);
	static void ApplyEffectToHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);
};
