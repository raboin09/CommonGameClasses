// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedActivation.h"
#include "ProjectileActivation.generated.h"

class ACommonProjectile;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UProjectileActivation : public URangedActivation
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UProjectileActivation();

protected:	
	virtual void Fire(int32 ActivationLevel = -1) override;
	virtual float GetOutlineRange() const override { return DEFAULT_OUTLINE_DISTANCE; }
	virtual ACommonProjectile* HandleProjectileFire();

	UFUNCTION(BlueprintNativeEvent, Category="Activation")
	TSubclassOf<ACommonProjectile> K2N_GetProjectileClassToSpawn() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Activation")
	TSubclassOf<ACommonProjectile> DefaultProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category="Activation", meta=(ClampMin = "1"))
	float ProjectileLife = 10.f;

private:
	virtual void Internal_AimAndShootProjectile(FVector& OutSpawnOrigin, FVector& OutVelocity);
	virtual ACommonProjectile* Internal_SpawnProjectile(const FVector& SpawnOrigin, const FVector& ProjectileVelocity);

	FORCEINLINE virtual TArray<TSubclassOf<AActor>> Internal_GetAdditionalEffectsToApplyToProjectile() const { return AbilityEffects; };
};
