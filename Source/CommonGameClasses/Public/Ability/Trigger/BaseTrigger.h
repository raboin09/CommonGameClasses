// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/TriggerMechanism.h"
#include "UObject/Object.h"
#include "BaseTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType)
class COMMONGAMECLASSES_API UBaseTrigger : public UObject, public ITriggerMechanism
{
	GENERATED_BODY()

public:
	virtual void PressTrigger() override PURE_VIRTUAL(UBaseTrigger::PressTrigger, )
	virtual void ReleaseTrigger() override PURE_VIRTUAL(UBaseTrigger::ReleaseTrigger, )
	virtual bool ShouldRetriggerAbilityAfterCooldown() const override PURE_VIRTUAL(UBaseTrigger::ShouldRetriggerAbilityAfterCooldown, return false; ) 
};
