// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterMovementComponent.h"
#include "CommonCharacterMovementComponent.generated.h"


class ACommonCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UCommonCharacterMovementComponent : public UAlsCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void SetWalkSpeedRatio(float Ratio);
	
private:
	UPROPERTY()
	ACommonCharacter* OwnerCharacter;
};
