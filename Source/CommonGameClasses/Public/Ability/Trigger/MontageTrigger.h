// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComplexTriggerBase.h"
#include "MontageTrigger.generated.h"

class UCharacterAnimationComponent;
struct FAnimMontagePlayData;

UCLASS(BlueprintType, Blueprintable)
class COMMONGAMECLASSES_API UMontageTrigger : public UComplexTriggerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger")
	TObjectPtr<UAnimMontage> MontageToPlay;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger")
	bool bHasCombos = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger|Combo", meta=(EditCondition = "bHasCombos"))
	int32 MaxComboSections = 3;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger|Combo", meta=(EditCondition = "bHasCombos"))
	bool bRandomizeMontageSection = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger|Combo", meta=(EditCondition = "bHasCombos"))
	FString ComboPrefix = "Combo";
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger")
	bool bShouldPlayerLockOnToNearestTarget = false;
	
protected:
	//~ Begin ITriggerMechanism Interface
	virtual void InitTriggerMechanism() override;
	virtual void ResetTrigger() override;
	FORCEINLINE virtual bool ShouldRetriggerAbilityAfterCooldown() const override { return false; }
	//~ End ITriggerMechanism Interface
	
	//~ Begin UAbilityTriggerBase Interface
	virtual void HandleSuccessfulTriggerPressed() override;
	virtual void HandleTriggerReleased() override;
	virtual int32 BPN_GetActivationLevel_Implementation() const override;
	//~ End UAbilityTriggerBase Interface
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Trigger")
	FName BPN_GetNextMontageSection() const;

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
