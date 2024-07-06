// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivationNotifyState.h"
#include "ActivateComboWindowNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UActivateComboWindowNotifyState : public UActivationNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	FGameplayTag AbilitySlotTag = CommonGameSlot::SlotMain;
};
