// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CostMechanism.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCostMechanism : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API ICostMechanism
{
	GENERATED_BODY()

public:
	virtual bool CanConsumeResource() PURE_VIRTUAL(ICostMechanism::CanConsumeResource, return false; )
	virtual bool TryReserveResource() PURE_VIRTUAL(ICostMechanism::TryReserveResource, return false; )
	virtual void ConsumeResource() PURE_VIRTUAL(ICostMechanism::ConsumeResource,)
	virtual void InitCostMechanism(APawn* InCharOwner) PURE_VIRTUAL(ICostMechanism::InitCostMechanism,)
};
