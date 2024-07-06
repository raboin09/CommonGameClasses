// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivation.h"
#include "AreaOfEffectActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UAreaOfEffectActivation : public UBaseActivation
{
	GENERATED_BODY()

protected:
	FORCEINLINE virtual float GetOutlineRange() const override { return DEFAULT_OUTLINE_DISTANCE; }
};
