// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedActivation.h"
#include "HitscanActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API AHitscanActivation : public ARangedActivation
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayTrailFX(const FVector& EndPoint);
	
	virtual void Fire(int32 ActivationLevel = -1) override;
	
private:
	void Internal_FireShot();
	void Internal_ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDirection);
	void Internal_PlayWeaponMissEffectFX(const FHitResult& Impact);
};
