﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ResourceContainer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UResourceContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IResourceContainer
{
	GENERATED_BODY()

public:
	virtual bool TrySpendResource(const float RequestedAmount) PURE_VIRTUAL(IResourceContainer::TrySpendResource, return false; )
	virtual void GiveResource(const float AmountToGive) PURE_VIRTUAL(IResourceContainer::GiveResource, )
};
