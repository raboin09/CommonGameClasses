﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Effect.generated.h"

UENUM(BlueprintType)
enum class EModifierOperator : uint8 
{
	None,
	Add,
	Subtract,
	Multiply,
};

UENUM(BlueprintType)
enum class EModifierActor : uint8 
{
	None,
	Instigator,
	Receiver
};

USTRUCT(BlueprintType)
struct FModifierExpression
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EModifierActor ModifierActor = EModifierActor::None;
	UPROPERTY(EditAnywhere)
	EModifierOperator Operation = EModifierOperator::None;
	UPROPERTY(EditAnywhere)
	float ModifierOperand = 1.f;
};

USTRUCT(BlueprintType)
struct FEffectContext
{
	GENERATED_BODY()

	FEffectContext()
	{
		InstigatingActor = nullptr;
		ReceivingActor = nullptr;
	}

	FEffectContext(AActor* InOriginator, AActor* InReceiver)
	{
		InstigatingActor = InOriginator;
		ReceivingActor = InReceiver;
	}

	// Actor that the effect is being applied to
	UPROPERTY(BlueprintReadOnly, Category="COMMON")
	AActor* ReceivingActor;
	// Actor that applied the effect
	UPROPERTY(BlueprintReadOnly, Category="COMMON")
	AActor* InstigatingActor;
	FHitResult SurfaceHit;
	FVector HitDirection;
};

UENUM()
enum class EEffectInterval : uint8
{
	Instant,
	Apply_Once,
	Apply_Every_Quarter_Second,
	Apply_Every_Half_Second,
	Apply_Every_Second,
	Apply_Every_Two_Seconds,
	Apply_Every_Three_Seconds,
	Apply_Every_Five_Seconds
};

USTRUCT()
struct FEffectValidTargets
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> RequiredTags;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> BlockedTags;
};

USTRUCT()
struct FEffectInitializationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM", meta=(EditCondition = "EffectInterval != EEffectInterval::Instant", EditConditionHides))
	bool bInfinite = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM", meta=(EditCondition = "EffectInterval != EEffectInterval::Instant", EditConditionHides))
	bool bEffectCanStack = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FEffectValidTargets ValidTargets;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM", meta=(ClampMin="1.0", EditCondition = "bInfinite != true && EffectInterval != EEffectInterval::Instant", EditConditionHides))
	double EffectDuration = 1.0;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	EEffectInterval EffectInterval = EEffectInterval::Instant;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> EffectTags;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> TagsToApply;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bShouldReverseEffectsOnDestroy = true;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> TagsToRemove;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> RemoveEffectsWithTags;
};

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
	virtual const FEffectInitializationData& GetEffectInitializationData() = 0;
	virtual const TArray<FGameplayTag>& GetBlockedTags() const = 0;
	virtual const TArray<FGameplayTag>& GetEffectTags() const = 0;
	virtual const TArray<FGameplayTag>& GetRequiredTags() const= 0;
	virtual const TArray<FGameplayTag>& GetRemoveEffectTags() const= 0;
	virtual void PlayEffectFX() PURE_VIRTUAL(IEffect::PlayEffectFX,)
	virtual void ActivateEffect() PURE_VIRTUAL(IEffect::ActivateEffect,)
	virtual void DestroyEffect() PURE_VIRTUAL(IEffect::DestroyEffect,)
};
