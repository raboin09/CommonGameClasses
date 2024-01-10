// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComplexTrigger.h"
#include "BaseTrigger.h"
#include "API/Ability/TriggerMechanism.h"
#include "MontageTrigger.generated.h"

struct FAnimMontagePlayData;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UMontageTrigger : public UBaseComplexTrigger
{
	GENERATED_BODY()

public:
	virtual void PressTrigger() override;
	virtual void ReleaseTrigger() override;
	virtual void ResetTrigger() override;
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return false; }

	UFUNCTION(BlueprintImplementableEvent)
	void K2_AfterPressedTrigger();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	UAnimMontage* MontageToPlay;
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	int32 MaxComboSections = 3;
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	bool bRandomizeMontages = false;
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	bool bShouldPlayerLockOnToNearestTarget = false;
	UPROPERTY(EditDefaultsOnly, Category="Trigger")
	FName CachedComboSection;
	
private:
	FAnimMontagePlayData Internal_GetPlayData() const;
	FName Internal_GetNextMontageSection() const;
	void Internal_StartMontage();

	// COMBO VARIABLES
	void Internal_IncrementComboCounter();
	void Internal_ResetComboCounter();
	
	int32 ComboSectionIncrement;
	const FString ComboPrefix = "Combo";
	FTimerHandle Timer_MaxComboWindow;
};
