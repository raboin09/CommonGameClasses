// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CommonEffectTypes.generated.h"

/**
 * Defines the mathematical operations that can be utilized within the effect system
 * to modify gameplay attributes or values. This enumeration determines the type of operation
 * applied during the modification process when a specific effect is being evaluated or executed.
 *
 * - None: No operation is applied.
 * - Add: Adds a specified value to an existing value.
 * - Subtract: Subtracts a specified value from an existing value.
 * - Multiply: Multiplies an existing value by a specified value.
 */
UENUM(BlueprintType)
enum class EModifierOperator : uint8 
{
	None,
	Add,
	Subtract,
	Multiply,
};

/**
 * Represents the classification of actors within the effect system for gameplay modifications.
 * This enum is used to identify whether an effect is associated with no specific actor,
 * the instigator (the actor triggering the effect), or the receiver (the actor affected by the effect).
 * It facilitates determining the actor context when evaluating gameplay conditions or effect applications.
 */
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

	/**
	 * Specifies the actor type being evaluated for a condition within the effect system.
	 * Determines whether the condition check is performed on the instigator (the actor
	 * initiating the effect) or the receiver (the target of the effect). This property
	 * is pivotal for defining the context in which a condition will be evaluated.
	 */
	UPROPERTY(EditAnywhere, DisplayName="Actor To Check Condition On")
	EModifierActor ModifierActor = EModifierActor::None;

	/**
	 * Defines the mathematical operation to be applied on the base value within the effect system.
	 * Determines how the modifier operand impacts the base value. Possible operations include
	 * addition, subtraction, and multiplication, providing flexibility in adjusting gameplay values.
	 */
	UPROPERTY(EditAnywhere)
	EModifierOperator Operation = EModifierOperator::None;

	/**
	 * Represents the operand value used within the effect system's modifier expressions.
	 * This numeric value is applied in conjunction with a specified operation (e.g., addition,
	 * subtraction, multiplication) to adjust calculated gameplay statistics or attributes.
	 * It acts as the magnitude or intensity of the modification for dynamic gameplay adjustments.
	 */
	UPROPERTY(EditAnywhere)
	float ModifierOperand = 1.f;
};

/**
 * Specifies the duration behavior of an effect within the system, determining its lifecycle
 * and how long it persists after being initiated.
 *
 * - Instant: The effect is applied and removed immediately after activation.
 * - Timed: The effect persists for a specified duration and is removed once the duration
 *   has elapsed.
 * - Infinite: The effect remains active indefinitely and is not automatically removed.
 * - Until_First_Successful_Activation: The effect remains active until its first successful
 *   activation, after which it is removed.
 */
UENUM()
enum class EEffectDurationType : uint8
{
	Instant UMETA(DisplayName = "Instant (Destroyed after activation)"),
	Timed UMETA(DisplayName = "Timed (Destroyed after duration)"),
	Infinite UMETA(DisplayName = "Infinite (Never destroyed of its own accord)"),
	Until_First_Successful_Activation UMETA(DisplayName = "Until First Successful Activation (Destroyed after activation)")
};

/**
 * Represents the intervals at which an effect is applied or executed within the effect system.
 * This enum defines both single-time application and periodic execution intervals, determining
 * the frequency of effect ticks during gameplay.
 *
 * - Apply_Once_Persistent: The effect is applied once and persists without recurring ticks.
 * - Apply_Every_Quarter_Second: The effect is applied every 0.25 seconds.
 * - Apply_Every_Half_Second: The effect is applied every 0.5 seconds.
 * - Apply_Every_Second: The effect is applied every 1 second.
 * - Apply_Every_Two_Seconds: The effect is applied every 2 seconds.
 * - Apply_Every_Three_Seconds: The effect is applied every 3 seconds.
 * - Apply_Every_Five_Seconds: The effect is applied every 5 seconds.
 */
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

	/**
	 * Represents the actor that serves as the recipient of an effect or action.
	 * Typically used to identify the target actor that an effect should influence
	 * or interact with during gameplay. This property allows for tracking or
	 * applying contextual logic for the effect's receiver.
	 */
	UPROPERTY(BlueprintReadOnly, Category="COMMON")
	TWeakObjectPtr<AActor> ReceivingActor;
	/**
	 * Represents the actor that originates or initiates an effect or action.
	 * Commonly used to track the source from which an effect is applied, such as
	 * the initiator in a gameplay interaction. This property enables logic
	 * related to the instigating actor, including conditional checks and context-specific
	 * operations during gameplay.
	 */
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

	/**
	 * Specifies a set of gameplay tags that are required to exist on a target for the effect to be applied.
	 * All tags in this array must be present on the target for the effect to activate successfully.
	 * This property facilitates implementing conditional requirements for the validity of targets.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON", DisplayName="Required Target Tags (All)")
	TArray<FGameplayTag> RequiredTags;

	/**
	 * Defines a set of gameplay tags that, if present on a target, will block the effect from being applied.
	 * If any of the tags in this array exist on the target, the effect activation will be prevented.
	 * This property allows implementing exclusions or restrictions for specific targets based on their tags.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON", DisplayName="Blocked Target Tags (Any)")
	TArray<FGameplayTag> BlockedTags;
};

USTRUCT(BlueprintType)
struct FEffectInitializationData
{
	GENERATED_BODY()

	/**
	 * Determines whether changes applied by the effect should be reverted once the effect is destroyed.
	 * This property is only applicable when the effect duration type is not set to 'Instant'.
	 * If true, any modifications made during the effect's activation will be reversed.
	 * If false, the changes will persist even after the effect ends.
	 */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "DurationType != EEffectDurationType::Instant"))
	bool bShouldReverseChangesAfterDestroy = true;

	/**
	 * Specifies the type of duration associated with the effect.
	 * This property determines how long the effect remains active.
	 * Possible values include:
	 * - Instant: The effect is applied immediately and has no persistence.
	 * - Timed: The effect is active for a specified duration before expiring.
	 * - Infinite: The effect lasts indefinitely until explicitly removed or replaced.
	 * The chosen value establishes whether the effect is temporary, permanent, or applied only once.
	 */
	UPROPERTY(EditDefaultsOnly)
	EEffectDurationType DurationType = EEffectDurationType::Instant;

	/**
	 * Indicates whether the effect can stack with other instances of the same effect on a target.
	 * When set to true, multiple instances of the effect can coexist, potentially intensifying
	 * the effect's impact. When false, only one instance of the effect can be active, and
	 * subsequent applications may overwrite or be ignored. This property is conditional
	 * and only valid for non-instant effects.
	 */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "DurationType != EEffectDurationType::Instant"))
	bool bEffectCanStack = false;

	/**
	 * Represents the set of conditions determining which targets are valid for an effect to activate.
	 * This includes tags that a target must possess to be considered eligible, as well as tags that
	 * disqualify the target if present. These constraints ensure the effect only applies to specific
	 * actors based on gameplay criteria.
	 */
	UPROPERTY(EditDefaultsOnly, meta=(ShowOnlyInnerProperties))
	FEffectValidTargets ValidTargets;

	/**
	 * Defines the duration, in seconds, that the effect remains active before it automatically expires.
	 * This property determines the lifetime for effects that are neither instant nor infinite in their duration.
	 * It represents a fixed period during which the effect can apply its functionality or modifications.
	 * For effects with duration types set to Instant or Infinite, this property is ignored.
	 */
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin="0.0", EditCondition = "DurationType != EEffectDurationType::Instant && DurationType != EEffectDurationType::Infinite"))
	double EffectDuration = 1.0;

	/**
	 * Specifies the interval at which the effect attempts to activate or apply its functionality.
	 * The tick interval determines the timing between consecutive activations for non-instant effects.
	 * It allows for recurring behavior while the effect is active, such as applying damage, healing,
	 * or other periodic updates. This property is ignored for effects with an instant duration.
	 */
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "DurationType != EEffectDurationType::Instant"))
	EEffectTickInterval TickInterval = EEffectTickInterval::Apply_Once_Persistent;

	/**
	 * Defines a collection of gameplay tags associated with the effect. These tags are used
	 * to represent the characteristics or conditions applied by the effect. Effect tags serve
	 * as identifiers that can be utilized for gameplay logic, such as determining interactions
	 * with other effects, systems, or game state. They are typically applied during the effect's
	 * activation and can influence how the effect operates or interacts with other game mechanics.
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EffectTags;

	/**
	 * Defines a collection of gameplay tags that are applied to the target entity when this effect is activated.
	 * These tags typically represent state changes (e.g., Buffs, Debuffs, or Conditions) and are used to help
	 * identify or enforce gameplay mechanics associated with the applied effect. The tags are added upon activation
	 * and may be automatically removed when the effect concludes, depending on if the effect reverses changes upon destruction.
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> TagsToApply;

	/**
	 * Specifies a collection of gameplay tags that identify effects to be removed when this effect is activated.
	 * Any ongoing effect associated with one or more tags from this list will be terminated.
	 * This mechanism is particularly useful for effects designed to cleanse or negate specific conditions
	 * (e.g., Poison, Burn), ensuring they are no longer active on the target.
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> TagsToRemove;

	/**
	 * A list of gameplay tags used to identify specific effects that should be removed
	 * when this effect is applied. Any effect containing one or more tags from this
	 * list will be targeted for removal. This feature can be useful for effects like
	 * "Cure" that remove detrimental effects (e.g., Poison, Burn) associated with certain tags.
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> RemoveEffectsWithTags;
};