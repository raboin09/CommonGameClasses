// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonCombatTypes.h"
#include "Types/CommonTypes.h"
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
	static void ApplyEffectsToHitResultsInRadius(AActor* InstigatingActor, TArray<TSubclassOf<AActor>> EffectsToApply, FVector TraceLocation, float TraceRadius, ETraceTypeQuery ValidationTraceType, bool bCanFriendlyFire, bool bValidateHit = true, FVector ValidationTraceStartLocation = FVector::ZeroVector, FName HitValidationBone = "spine_02");
	static void ApplyEffectAtLocation(AActor* InstigatingActor, TSubclassOf<AActor> EffectToApply, FVector Location, bool bActivateImmediately = true);
	UFUNCTION(BlueprintCallable, Category="COMMON|Utils")
	static void ApplyEffectToActor(AActor* ReceivingActor, TSubclassOf<AActor> EffectToApply);
	static void ApplyEffectsToActor(TArray<TSubclassOf<AActor>> EffectsToApply, AActor* ReceivingActor);
	static void ApplyEffectsToHitResult(TArray<TSubclassOf<AActor>> EffectsToApply, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);
	static void ApplyEffectToHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);

	/////////////////////////////////////////////////////////////
	// Applies to both HealthComponent and ShieldEnergyComponent
	/////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category="COMMON|Utils")
	static void TryApplyDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent = FDamageHitReactEvent());
	
	/////////////////////////////////////////////////////
	// ONLY applies to HealthComponent
	/////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category="COMMON|Utils")
	static void TryApplyHealthDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent = FDamageHitReactEvent());
	UFUNCTION(BlueprintCallable, Category="COMMON|Utils")
	static void TryApplyHealthHealToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Heal);
	UFUNCTION(BlueprintCallable, Category="COMMON|Utils")
	static void TryAddMaxWoundsToActor(const AActor* ReceivingActor, float MaxWoundsToAdd);

	/////////////////////////////////////////////////////
	// ONLY applies to ShieldEnergyComponent
	/////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category="COMMON|Utils")
	static void TryApplyShieldDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent = FDamageHitReactEvent());
	UFUNCTION(BlueprintCallable, Category="COMMON|Utils")
	static void TryApplyShieldHealToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Heal);
};
