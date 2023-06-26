// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/AbilityComponent.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ActivationNotifyState.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API UActivationNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	UAbilityComponent* GetAbilityComponentFromOwner(const USkeletalMeshComponent* MeshComp) const;
};
