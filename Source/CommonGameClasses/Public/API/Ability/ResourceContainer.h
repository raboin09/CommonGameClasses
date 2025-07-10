// Fill out your copyright notice in the Description page of Project Settings.

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
	virtual bool TryConsumeResourceAmount(const float RequestedAmount) PURE_VIRTUAL(IResourceContainer::TryConsumeResourceAmount, return false; )
	virtual void TryGiveResourceAmount(const float AmountToGive) PURE_VIRTUAL(IResourceContainer::TryGiveResourceAmount, )
	virtual void TryTogglePauseResourceRegen(bool bShouldPause) PURE_VIRTUAL(IResourceContainer::TryTogglePauseResourceRegen, )
	
	virtual float GetAvailableResourceAmount() const PURE_VIRTUAL(IResourceContainer::GiveResource, return -1.f; )
	virtual float GetMaxResourceAmount() const PURE_VIRTUAL(IResourceContainer::GetMaxResourceAmount, return -1.f; )
};
