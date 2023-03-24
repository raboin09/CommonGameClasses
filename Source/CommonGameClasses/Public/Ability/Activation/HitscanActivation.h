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
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_PlayTrailFX(const FVector& EndPoint);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void K2_ProcessInstantHit(const FHitResult& Impact);
	
	virtual void Fire(int32 ActivationLevel = -1) override;
	
private:
	void Internal_FireShot();
	void Internal_PlayWeaponMissEffectFX(const FHitResult& Impact);
};
