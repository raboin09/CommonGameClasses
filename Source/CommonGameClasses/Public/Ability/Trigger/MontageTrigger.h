﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComplexTrigger.h"
#include "MontageTrigger.generated.h"

class UCharacterAnimationComponent;
struct FAnimMontagePlayData;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UMontageTrigger : public UBaseComplexTrigger
{
	GENERATED_BODY()
	
protected:
	// ITriggerMechanism overrides
	virtual void InitTriggerMechanism() override;
	virtual void ResetTrigger() override;
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return false; }
	
	// UBaseTrigger overrides
	virtual void HandleSuccessfulTriggerPressed() override;
	virtual void HandleTriggerReleased() override;

	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Trigger")
	FName K2N_GetNextMontageSection() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	TObjectPtr<UAnimMontage> MontageToPlay;
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	bool bHasCombos = false;
	UPROPERTY(EditDefaultsOnly, Category="Trigger|Combo", meta=(EditCondition = "bHasCombos"))
	int32 MaxComboSections = 3;
	UPROPERTY(EditDefaultsOnly, Category="Trigger|Combo", meta=(EditCondition = "bHasCombos"))
	bool bRandomizeMontageSection = false;
	UPROPERTY(EditDefaultsOnly, Category="Trigger|Combo", meta=(EditCondition = "bHasCombos"))
	FString ComboPrefix = "Combo";
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	bool bShouldPlayerLockOnToNearestTarget = false;
	
private:
	UFUNCTION()
	void HandleMontageEnded(const FCharacterMontageEndedPayload& CharacterMontageEndedPayload);
	
	FAnimMontagePlayData Internal_GetPlayData() const;
	void Internal_StartMontage();

	TWeakObjectPtr<UCharacterAnimationComponent> CharacterAnimationComponent;
	
	// COMBO VARIABLES
	void Internal_IncrementComboCounter();
	void Internal_ResetComboCounter();
	
	int32 ComboSectionIncrement;
};
