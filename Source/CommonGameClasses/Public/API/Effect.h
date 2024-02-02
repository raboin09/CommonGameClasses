// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEffectTypes.h"
#include "UObject/Interface.h"
#include "Effect.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEffect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IEffect
{
	GENERATED_BODY()

public:
	virtual void SetEffectContext(const FEffectContext& InContext) PURE_VIRTUAL(IEffect::SetEffectContext,)
	virtual FEffectContext GetEffectContext() const PURE_VIRTUAL(IEffect::GetEffectContext, return FEffectContext();)
	
	virtual const FEffectInitializationData& GetEffectInitializationData() = 0;
	virtual const TArray<FGameplayTag>& GetEffectTags() const = 0;
	virtual const TArray<FGameplayTag>& GetRemoveEffectTags() const= 0;
	
	virtual void PlayEffectFX() PURE_VIRTUAL(IEffect::PlayEffectFX,)
	virtual bool TryActivateEffect() PURE_VIRTUAL(IEffect::TryActivateEffect, return false; )
	virtual void DestroyEffect() PURE_VIRTUAL(IEffect::DestroyEffect,)
};
