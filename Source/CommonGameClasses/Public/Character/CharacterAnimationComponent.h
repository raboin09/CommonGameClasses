// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAnimationComponent.generated.h"

struct FAnimMontagePlayData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UCharacterAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAnimationComponent();
	float TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData);
};
