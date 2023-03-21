// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedActivation.h"
#include "ProjectileActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API AProjectileActivation : public ARangedActivation
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileActivation();

protected:	
	virtual void Fire(int32 ActivationLevel = -1) override;
	virtual class ACommonProjectile* HandleProjectileFire();
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSubclassOf<ACommonProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON", meta=(ClampMin = "1", EditCondition = "ProjectileClass != nullptr", EditConditionHides))
	float ProjectileLife = 10.f;

private:
	FORCEINLINE virtual TArray<TSubclassOf<AActor>> Internal_GetAdditionalEffectsToApplyToProjectile() const { return AbilityEffects; };
	virtual void Internal_AimAndShootProjectile(FVector& OutSpawnOrigin, FVector& OutVelocity);
	virtual ACommonProjectile* Internal_SpawnProjectile(const FVector& SpawnOrigin, const FVector& ProjectileVelocity);
};
