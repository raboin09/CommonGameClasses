// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Effect.h"
#include "UObject/Object.h"
#include "ConditionTree.generated.h"

UENUM(BlueprintType)
enum class ETagOperator : uint8
{
	None UMETA(DisplayName = "None"),
	HasTag UMETA(DisplayName = "Has Gameplay Tag"),
	HasTagsAny UMETA(DisplayName = "Has Gameplay Tags (ANY)"),
	HasTagsAll UMETA(DisplayName = "Has Gameplay Tags (ALL)"),
};

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

UENUM(BlueprintType)
enum class ETreeOperator : uint8
{
	None,
	And UMETA(DisplayName = "AND"),
	Or UMETA(DisplayName = "OR"),
	Not UMETA(DisplayName = "NOT"),
};

UENUM(BlueprintType)
enum class EPropertyOwner : uint8
{
	None,
	Instigator,
	ReceivingActor
};

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
	AActor* GetPropertyOwner(EPropertyOwner PropertyOwner, const FEffectContext& InEffectContext) const;
};

UCLASS()
class COMMONGAMECLASSES_API UConditionTreeNode : public UConditionExpression
{
	GENERATED_BODY()

public:
	virtual bool AreConditionsTrue(const FEffectContext& InEffectContext) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Condition Tree", meta=(EditCondition = "ChildConditions.Num() > 0", EditConditionHides))
	ETreeOperator Operator;
	UPROPERTY(EditDefaultsOnly, Category="Condition Tree", Instanced)
	TArray<UConditionExpression*> ChildConditions;
};

UCLASS(Abstract)
class COMMONGAMECLASSES_API UConditionLeafNode : public UConditionExpression
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Condition")
	EPropertyOwner PropertyOwner;
};

UCLASS()
class COMMONGAMECLASSES_API UFloatLeafConditionNode : public UConditionLeafNode
{
	GENERATED_BODY()

protected:
	virtual bool AreConditionsTrue(const FEffectContext& InEffectContext) const override;
	float GetProperty(const FEffectContext& InEffectContext) const;

	UPROPERTY(EditDefaultsOnly, Category="Condition")
	EMathLeafOperator Operator;
	UPROPERTY(EditDefaultsOnly, Category="Condition")
	EFloatOperand FloatProperty;
	UPROPERTY(EditDefaultsOnly, Category="Condition")
	float Value;
};

UCLASS()
class COMMONGAMECLASSES_API UGameplayTagConditionNode : public UConditionLeafNode
{
	GENERATED_BODY()

protected:
	virtual bool AreConditionsTrue(const FEffectContext& InEffectContext) const override;

	UPROPERTY(EditDefaultsOnly, Category="Condition")
	ETagOperator Operator;
	UPROPERTY(EditDefaultsOnly, Category="Condition")
	TArray<FGameplayTag> ConditionTags;
	UPROPERTY(EditDefaultsOnly, Category="Condition")
	bool bExactMatch = false;
};