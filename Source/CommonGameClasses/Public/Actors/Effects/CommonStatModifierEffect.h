﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEffect.h"
#include "Types/CommonCombatTypes.h"
#include "CommonStatModifierEffect.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class UBaseStatsModifierData : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseStatsModifierData();

	UPROPERTY(EditDefaultsOnly)
	EEffectStatType StatToModify = EEffectStatType::Damage_All;
	UPROPERTY(EditDefaultsOnly)
	float BaseModifierValue;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "StatToModify == EEffectStatType::Damage", EditConditionHides))
	bool bAddDamageForHeadshots = false;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "StatToModify == EEffectStatType::Damage && bAddDamageForHeadshots", EditConditionHides))
	float HeadshotModifier = 1.f;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "StatToModify == EEffectStatType::Damage", EditConditionHides))
	EHitReactType HitImpulse;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "StatToModify == EEffectStatType::Damage", EditConditionHides))
	EHitReactType DeathImpulse;
};

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonStatModifierEffect : public ACommonEffect
{
	GENERATED_BODY()
	
protected:
	virtual bool TryActivateEffect() override;
	virtual void DestroyEffect() override;

	// Optionally override one of these in child classes
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void K2_ApplyStatChange(float ModifiedStatValue);
	virtual void K2_ApplyStatChange_Implementation(float ModifiedStatValue);

	UPROPERTY(EditDefaultsOnly, Instanced, Category="CUSTOM")
	UBaseStatsModifierData* StatEffectDataObj;

	TFunction<void()> ReversalFunc;
	
private:
	float CalculateHeadshotDamage(float ModifiedStatValue) const;
	float CalculateModifierValues() const;
	void CalculateModifier(const FModifierExpression& ModifierExpression, const FGameplayTag& ModifierTag, float& ModifiedBaseValue) const;

	void Internal_MovespeedStatChange(float ModifiedStatValue);
	void Internal_AllDamage(float ModifiedStatValue) const;
	void Internal_HealthDamage(float ModifiedStatValue) const;
	void Internal_ShieldDamage(float ModifiedStatValue) const;
	void Internal_HealthHeal(float ModifiedStatValue) const;
	void Internal_ShieldHeal(float ModifiedStatValue) const;
	void Internal_HealthMaxWounds(float ModifiedStatValue);
	FDamageHitReactEvent Internal_GenerateHitReactEvent(float ModifiedStatValue) const;
};
