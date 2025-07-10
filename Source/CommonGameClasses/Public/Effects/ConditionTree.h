// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Effect.h"
#include "UObject/Object.h"
#include "ConditionTree.generated.h"

/**
 * An enumeration that defines various gameplay tag operation types, which
 * are used to evaluate actor or gameplay-related tag conditions in the
 * context of a condition or rule system.
 */
UENUM(BlueprintType)
enum class ETagOperator : uint8
{
	None UMETA(DisplayName = "None"),
	HasTag UMETA(DisplayName = "Has Gameplay Tag"),
	HasTagsAny UMETA(DisplayName = "Has Gameplay Tags (ANY)"),
	HasTagsAll UMETA(DisplayName = "Has Gameplay Tags (ALL)"),
};

/**
 * An enumeration representing possible mathematical comparison operators
 * that can be used to evaluate conditions involving numerical types.
 * These operators allow for comparisons such as equality, inequality,
 * or relational evaluations between two values.
 */
UENUM(BlueprintType)
enum class EMathLeafOperator : uint8
{
	None UMETA(DisplayName = "None"),
	Equal UMETA(DisplayName = "=="),
	NotEqual UMETA(DisplayName = "!="),
	GreaterThan UMETA(DisplayName = ">"),
	GreaterThanOrEqual UMETA(DisplayName = ">="),
	LessThan UMETA(DisplayName = "<"),
	LessThanOrEqual UMETA(DisplayName = "<="),
};

/**
 * An enumeration defining logical operators used for evaluating conditions
 * in a condition tree structure. These operators determine how child conditions
 * are combined or modified when evaluating the overall condition's truth value.
 */
UENUM(BlueprintType)
enum class ETreeOperator : uint8
{
	None,
	And UMETA(DisplayName = "AND"),
	Or UMETA(DisplayName = "OR"),
	Not UMETA(DisplayName = "NOT"),
};

/**
 * An enumeration that specifies ownership contexts for accessing property values
 * within a gameplay system. This helps determine the actor or context for which
 * a property operation is executed, such as the instigator of an action or the
 * receiving actor affected by the action.
 */
UENUM(BlueprintType)
enum class EPropertyOwner : uint8
{
	None,
	Instigator,
	ReceivingActor
};

/**
 * An enumeration that represents types of float-derived operands that can be
 * evaluated in conditional logic. These operands are used to retrieve specific
 * property values from actors, such as health or shield status, during gameplay.
 */
UENUM(BlueprintType)
enum class EFloatOperand : uint8
{
	None,
	CurrentHealth,
	CurrentShield
};

UCLASS(Abstract, NotBlueprintable, BlueprintType, EditInlineNew)
class COMMONGAMECLASSES_API UConditionExpression : public UObject
{
	GENERATED_BODY()

public:
	virtual bool AreConditionsTrue(const FEffectContext& InEffectContext) const PURE_VIRTUAL(UConditionExpression::AreConditionsTrue, return false; )

protected:
	TWeakObjectPtr<AActor> GetPropertyOwner(EPropertyOwner PropertyOwner, const FEffectContext& InEffectContext) const;
};

UCLASS()
class COMMONGAMECLASSES_API UConditionTreeNode : public UConditionExpression
{
	GENERATED_BODY()

public:
	virtual bool AreConditionsTrue(const FEffectContext& InEffectContext) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	ETreeOperator Operator;
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UConditionExpression>> ChildConditions;
};

UCLASS(Abstract)
class COMMONGAMECLASSES_API UConditionLeafNode : public UConditionExpression
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	EPropertyOwner PropertyOwner;
};

UCLASS()
class COMMONGAMECLASSES_API UFloatLeafConditionNode : public UConditionLeafNode
{
	GENERATED_BODY()

protected:
	virtual bool AreConditionsTrue(const FEffectContext& InEffectContext) const override;
	float GetProperty(const FEffectContext& InEffectContext) const;

	UPROPERTY(EditDefaultsOnly)
	EMathLeafOperator Operator;
	UPROPERTY(EditDefaultsOnly)
	EFloatOperand FloatProperty;
	UPROPERTY(EditDefaultsOnly)
	float Value;
};

UCLASS()
class COMMONGAMECLASSES_API UGameplayTagConditionNode : public UConditionLeafNode
{
	GENERATED_BODY()

protected:
	virtual bool AreConditionsTrue(const FEffectContext& InEffectContext) const override;

	UPROPERTY(EditDefaultsOnly)
	ETagOperator Operator;
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> ConditionTags;
	UPROPERTY(EditDefaultsOnly)
	bool bExactMatch = false;
};