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
	virtual ACommonProjectile* HandleProjectileFire();
	
	UPROPERTY(EditDefaultsOnly, Category="Activation")
	TSubclassOf<ACommonProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category="Activation", meta=(ClampMin = "1", EditCondition = "ProjectileClass != nullptr", EditConditionHides))
	float ProjectileLife = 10.f;

private:
	virtual void Internal_AimAndShootProjectile(FVector& OutSpawnOrigin, FVector& OutVelocity);
	virtual ACommonProjectile* Internal_SpawnProjectile(const FVector& SpawnOrigin, const FVector& ProjectileVelocity);

	FORCEINLINE virtual TArray<TSubclassOf<AActor>> Internal_GetAdditionalEffectsToApplyToProjectile() const { return AbilityEffects; };
};
