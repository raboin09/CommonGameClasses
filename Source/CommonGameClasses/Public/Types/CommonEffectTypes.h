// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CommonEffectTypes.generated.h"

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

	// Which actor to check the expression on. For example, if we want to check if the receiving actor has the "Weakened" tag
	// we would set it to Receiver. 
	UPROPERTY(EditAnywhere, DisplayName="Actor To Check Condition On")
	EModifierActor ModifierActor = EModifierActor::None;
	// How to modify the effect scalar if the ModifierActor condition is true. If an effect has Damage, this will choose how to adjust the
	// Damage value based on the conditions (e.g. if the Receiving actor is "Weakened", set this to Multiply and set the Operand to 1.25).
	UPROPERTY(EditAnywhere)
	EModifierOperator Operation = EModifierOperator::None;
	UPROPERTY(EditAnywhere)
	float ModifierOperand = 1.f;
};

UENUM()
enum class EEffectDurationType : uint8
{
	Instant UMETA(DisplayName = "Instant (Destroyed after activation)"),
	Timed UMETA(DisplayName = "Timed (Destroyed after duration)"),
	Infinite UMETA(DisplayName = "Infinite (Never destroyed of its own accord)"),
	Until_First_Successful_Activation UMETA(DisplayName = "Until First Successful Activation (Destroyed after activation)")
};

UENUM()
enum class EEffectTickInterval : uint8
{
	Apply_Once_Persistent UMETA(DisplayName = "Apply once (persistent)"),
	Apply_Every_Quarter_Second UMETA(DisplayName = "Apply every .25 seconds"),
	Apply_Every_Half_Second UMETA(DisplayName = "Apply every .5 seconds"),
	Apply_Every_Second UMETA(DisplayName = "Apply every second"),
	Apply_Every_Two_Seconds UMETA(DisplayName = "Apply every two seconds"),
	Apply_Every_Three_Seconds UMETA(DisplayName = "Apply every three seconds"),
	Apply_Every_Five_Seconds UMETA(DisplayName = "Apply every 5 seconds")
};

USTRUCT(BlueprintType)
struct FEffectContext
{
	GENERATED_BODY()

	FEffectContext()
	{
		InstigatingActor = nullptr;
		ReceivingActor = nullptr;
		HitDirection = FVector::ZeroVector;
	}

	FEffectContext(AActor* InOriginator, AActor* InReceiver)
	{
		InstigatingActor = InOriginator;
		ReceivingActor = InReceiver;
		HitDirection = FVector::ZeroVector;
	}

	// Actor that the effect is being applied to
	UPROPERTY(BlueprintReadOnly, Category="COMMON")
	TWeakObjectPtr<AActor> ReceivingActor;
	// Actor that applied the effect
	UPROPERTY(BlueprintReadOnly, Category="COMMON")
	TWeakObjectPtr<AActor> InstigatingActor;
	UPROPERTY(BlueprintReadOnly, Category="COMMON")
	FHitResult SurfaceHit;
	UPROPERTY(BlueprintReadOnly, Category="COMMON")
	FVector HitDirection;
};

USTRUCT()
struct FEffectValidTargets
{
	GENERATED_BODY()

	// Tags that the receiving actor needs for the effect to be activated 
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM", DisplayName="Required Target Tags (All)")
	TArray<FGameplayTag> RequiredTags;
	// Tags that the actors CANNOT have in order for the effect to be activated
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM", DisplayName="Blocked Target Tags (Any)")
	TArray<FGameplayTag> BlockedTags;
};

USTRUCT(BlueprintType)
struct FEffectInitializationData
{
	GENERATED_BODY()

	// Whether to remove the TagsToApply and reverse any other changes made when the effect is destroyed.
	// So if there is a temporary MoveSpeed boost, this is true. If it's a permanent HP boost, it's false.
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "DurationType != EEffectDurationType::Instant"))
	bool bShouldReverseChangesAfterDestroy = true;
	UPROPERTY(EditDefaultsOnly)
	EEffectDurationType DurationType = EEffectDurationType::Instant;
	// If true, new effects of the same class can exist in the same effect container. If false, the effect duration will reset and just 1 effect of this class will remain.
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "DurationType != EEffectDurationType::Instant"))
	bool bEffectCanStack = false;
	// Required/Blocked tags that the receiving Actor must (not) have in order to activate this effect
	UPROPERTY(EditDefaultsOnly, meta=(ShowOnlyInnerProperties))
	FEffectValidTargets ValidTargets;

	// How long the effect ticks (and tries to activate)
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="1.0", EditCondition = "DurationType != EEffectDurationType::Instant && DurationType != EEffectDurationType::Infinite"))
	double EffectDuration = 1.0;	
	// How often the effect ticks in the effect container
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "DurationType != EEffectDurationType::Instant"))
	EEffectTickInterval TickInterval = EEffectTickInterval::Apply_Once_Persistent;
	
	// These tags arent applied, they are simply descriptors of the tag (e.g. Poison, CrowdControl, etc).
	// Useful for abilities like "Cure" that remove all effects with the "Poison" tag.
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EffectTags;
	// Tags that are applied to the target actor (e.g. apply the "Stunned" tag so the character can handle the stunned state).
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> TagsToApply;
	// Tags that are removed from the target actor (e.g. remove the "Stunned" tag so it frees the character to move again).
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> TagsToRemove;
	// Remove all effects whose EffectTags match any one of these (e.g. in a Cure effect, remove any/all effects with the EffectTag of "Poison").
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> RemoveEffectsWithTags;
	
};