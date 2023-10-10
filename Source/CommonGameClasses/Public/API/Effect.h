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
enum class EEffectDurationType : uint8
{
	Instant UMETA(DisplayName = "Instant (Destroyed after activation)"),
	Timer UMETA(DisplayName = "Timed (Destroyed after duration)"),
	Infinite UMETA(DisplayName = "Infinite (Never destroyed)"),
	FirstActivation UMETA(DisplayName = "Until First Successful Activation (Destroyed after activation)")
};

UENUM()
enum class EEffectTickInterval : uint8
{
	Apply_Once UMETA(DisplayName = "Apply once"),
	Apply_Every_Quarter_Second UMETA(DisplayName = "Apply every .25 seconds"),
	Apply_Every_Half_Second UMETA(DisplayName = "Apply every .5 seconds"),
	Apply_Every_Second UMETA(DisplayName = "Apply every second"),
	Apply_Every_Two_Seconds UMETA(DisplayName = "Apply every two seconds"),
	Apply_Every_Three_Seconds UMETA(DisplayName = "Apply every three seconds"),
	Apply_Every_Five_Seconds UMETA(DisplayName = "Apply every 5 seconds")
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
	UPROPERTY(EditDefaultsOnly)
	EEffectDurationType DurationType = EEffectDurationType::Instant;
	// If false, the effect won't reset. If true, the effect duration will reset.
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "DurationType != EEffectDurationType::Instant"))
	bool bEffectCanStack = false;
	// Required/Blocked tags that the receiving Actor must (not) have in order to activate this effect
	UPROPERTY(EditDefaultsOnly)
	FEffectValidTargets ValidTargets;

	// How long the effect ticks (and tries to activate)
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="1.0", EditCondition = "DurationType != EEffectDurationType::Instant"))
	double EffectDuration = 1.0;	
	// How often the effect ticks in the effect container
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="1.0", EditCondition = "DurationType != EEffectDurationType::Instant"))
	EEffectTickInterval TickInterval = EEffectTickInterval::Apply_Every_Second;
	
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
