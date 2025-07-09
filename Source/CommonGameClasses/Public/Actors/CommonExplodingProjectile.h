// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonProjectile.h"
#include "CommonExplodingProjectile.generated.h"

UCLASS()
class COMMONGAMECLASSES_API ACommonExplodingProjectile : public ACommonProjectile
{
	GENERATED_BODY()
	
public:
	ACommonExplodingProjectile();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void HandleActorDeath() override;
	virtual void BPN_HandleImpact_Implementation(const FHitResult& HitResult) override;
	virtual void OnImpact(const FHitResult& HitResult) override;

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleExplosion();
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Explosion")
	float ExplosionRadius = 512.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Explosion")
	bool bCanFriendlyFire = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Explosion")
	bool bExplosionRequiresVisibilityValidation = true;

private:
	void Internal_Explode();

	UPROPERTY(Transient)
	bool bExplodedAlready = false;
};
