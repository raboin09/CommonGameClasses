// Fill out your copyright notice in the Description page of Project Settings.

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
	// Resolves instantly and is then destroyed immediately afterward (never ticks)
	Instant UMETA(DisplayName = "Instant (Destroyed After Activation)"),
	// Added to tick container and applies only ONCE and lasts for the duration
	Apply_Once UMETA(DisplayName = "Just Once"),
	Apply_Every_Quarter_Second UMETA(DisplayName = "Every .25 seconds"),
	Apply_Every_Half_Second UMETA(DisplayName = "Every .5 seconds"),
	Apply_Every_Second UMETA(DisplayName = "Every second"),
	Apply_Every_Two_Seconds UMETA(DisplayName = "Every two seconds"),
	Apply_Every_Three_Seconds UMETA(DisplayName = "Every three seconds"),
	Apply_Every_Five_Seconds UMETA(DisplayName = "Every 5 seconds")
};

USTRUCT()
struct FEffectValidTargets
{
	GENERATED_BODY()

	// Tags that the receiving actor needs for the effect to be activated 
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> RequiredTags;
	// Tags that the actos CANNOT have in order for the effect to be activated
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TArray<FGameplayTag> BlockedTags;
};

USTRUCT(BlueprintType)
struct FEffectInitializationData
{
	GENERATED_BODY()

	// Whether to remove the TagsToApply and reverse any other changes made when the effect is destroyed.
	// So if there is a temporary MoveSpeed boost, this is true. If it's a permanent HP boost, it's false.
	UPROPERTY(EditDefaultsOnly)
	bool bShouldReverseChangesAfterDestroy = true;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "EffectInterval != EEffectInterval::Instant", EditConditionHides))
	bool bEffectCanStack = false;
	// If true, this shouldn't be "Just Once" as it shouldn't activate right away, it checks for activation over the duration.
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "EffectInterval != EEffectInterval::Instant", EditConditionHides))
	bool bDestroyAfterFirstActivation = false;
	
	UPROPERTY(EditDefaultsOnly)
	FEffectValidTargets ValidTargets;

	// If the duration of the effect is infinite or has an expiration
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "EffectInterval != EEffectInterval::Instant", EditConditionHides))
	bool bInfinite = false;
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="1.0", EditCondition = "bInfinite != true && EffectInterval != EEffectInterval::Instant", EditConditionHides))
	double EffectDuration = 1.0;
	
	// How often the effect ticks in the effect container
	UPROPERTY(EditDefaultsOnly)
	EEffectInterval EffectInterval = EEffectInterval::Instant;
	
	// These tags arent applied, they are simply descriptors of the tag (e.g. Poison, CrowdControl, etc).
	// Useful for abilities like "Cure" that remove all effects with the "Poison" tag.
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EffectTags;
	// Tags that are applied to the target actor
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> TagsToApply;
	// Tags that are removed from the target actor
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> TagsToRemove;
	// Remove all effects whose EffectTags match any one of these
	UPROPERTY(EditDefaultsOnly)
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
	virtual FEffectContext GetEffectContext() const PURE_VIRTUAL(IEffect::GetEffectContext, return FEffectContext();)
	
	virtual const FEffectInitializationData& GetEffectInitializationData() = 0;
	virtual const TArray<FGameplayTag>& GetEffectTags() const = 0;
	virtual const TArray<FGameplayTag>& GetRemoveEffectTags() const= 0;
	
	virtual void PlayEffectFX() PURE_VIRTUAL(IEffect::PlayEffectFX,)
	virtual bool TryActivateEffect() PURE_VIRTUAL(IEffect::TryActivateEffect, return false; )
	virtual void DestroyEffect() PURE_VIRTUAL(IEffect::DestroyEffect,)
};
