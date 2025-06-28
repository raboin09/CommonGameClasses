// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedActivation.h"
#include "HitscanActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UHitscanActivation : public URangedActivation
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_PlayTrailFX(const FVector& StartPoint, const FVector& EndPoint);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_OnSuccessfulHit(const FHitResult& Impact);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Ability")
	void BPN_ProcessInstantHit(const FHitResult& Impact);
	
	virtual void Fire(const FTriggerEventPayload& TriggerEventPayload) override;
	
private:
	void Internal_FireShot();
	void Internal_PlayWeaponMissEffectFX(const FHitResult& Impact);
};
