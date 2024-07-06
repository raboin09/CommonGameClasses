// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivationNotifyState.h"
#include "ActivateAbilityNotifyState.generated.h"

UCLASS()
class COMMONGAMECLASSES_API UActivateAbilityNotifyState : public UActivationNotifyState 
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category="CUSTOM", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSoftClassPtr<AActor>> ActivationEffects;
};