// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityOutliner.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAbilityOutliner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IAbilityOutliner
{
	GENERATED_BODY()

public:
	// If this is equipped, the PlayerController may need this to know if it should outline an interaction Actor or not
	virtual float GetAbilityOutlineRange() const PURE_VIRTUAL(IAbilityOutliner::GetAbilityOutlineRange, return -1.f; )
};
