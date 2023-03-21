// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/TriggerMechanism.h"
#include "BurstTrigger.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ABurstTrigger : public ACommonActor, public ITriggerMechanism
{
	GENERATED_BODY()
public:
	ABurstTrigger();
	virtual void PressTrigger() override;
	virtual void ReleaseTrigger() override;
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return bTriggerHeld; }

protected:
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	int32 NumberOfShotsPerFire = 3;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability")
	float TimeBetweenBurstShots = .1f;
	
private:	
	UFUNCTION()
	void Internal_BurstFireTick();

	bool bTriggerHeld;
	int32 BurstFireCount;
	FTimerHandle TimerHandle_BurstFire;
};
