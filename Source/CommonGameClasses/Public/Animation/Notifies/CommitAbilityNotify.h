// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivationNotifyState.h"
#include "CommitAbilityNotify.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommitAbilityNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
