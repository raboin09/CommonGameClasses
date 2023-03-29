// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CombatTypes.h"
#include "Types/CommonTypes.h"
#include "Types/ResourceTypes.h"
#include "UObject/Object.h"
#include "EffectUtils.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UEffectUtils : public UObject
{
	GENERATED_BODY()

public:
	static void ApplyEffectsToHitResultsInRadius(AActor* InstigatingActor, TArray<TSubclassOf<AActor>> EffectsToApply, FVector TraceLocation, float TraceRadius, ETraceTypeQuery ValidationTraceType, EAffiliation AffectedAffiliation = EAffiliation::All, bool bValidateHit = true, FVector ValidationTraceStartLocation = FVector::ZeroVector, FName HitValidationBone = "spine_02");
	static void ApplyEffectAtLocation(AActor* InstigatingActor, TSubclassOf<AActor> EffectToApply, FVector Location, bool bActivateImmediately = true);
	UFUNCTION(BlueprintCallable)
	static void ApplyEffectToActor(AActor* ReceivingActor, TSubclassOf<AActor> EffectToApply);
	static void ApplyEffectsToActor(TArray<TSubclassOf<AActor>> EffectsToApply, AActor* ReceivingActor);
	static void ApplyEffectsToHitResult(TArray<TSubclassOf<AActor>> EffectsToApply, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);
	static void ApplyEffectToHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);

	static void TryAddMaxWoundsToActor(const AActor* ReceivingActor, float MaxWoundsToAdd);
	static void TryApplyHealToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Heal);
	UFUNCTION(BlueprintCallable)
	static void TryApplyDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent = FDamageHitReactEvent());

	static class UFXSystemAsset* GetVFXAssetFromKey(const struct FDataTableRowHandle& RowHandle, const UPhysicalMaterial* SurfaceMaterial, bool bIsValidHeadshot);
	static class USoundCue* GetSFXAssetFromKey(const FDataTableRowHandle& RowHandle, const UPhysicalMaterial* SurfaceMaterial, bool bIsValidHeadshot);

};
