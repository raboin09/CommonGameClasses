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
	virtual void Fire(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual float GetOutlineRange() const override { return DEFAULT_OUTLINE_DISTANCE; }
	virtual ACommonProjectile* HandleProjectileFire(const FTriggerEventPayload& TriggerEventPayload);

	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Activation")
	TSubclassOf<ACommonProjectile> BPN_GetProjectileClassToSpawn(const FTriggerEventPayload& TriggerEventPayload) const;
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	TSubclassOf<ACommonProjectile> DefaultProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(ClampMin = "1"))
	float ProjectileLife = 10.f;

private:
	virtual void Internal_AimAndShootProjectile(FVector& OutSpawnOrigin, FVector& OutVelocity);
	virtual ACommonProjectile* Internal_SpawnProjectile(const FVector& SpawnOrigin, const FVector& ProjectileVelocity, const FTriggerEventPayload& TriggerEventPayload);

	FORCEINLINE virtual TArray<TSubclassOf<AActor>> Internal_GetAdditionalEffectsToApplyToProjectile() const { return AbilityEffects; };
};
