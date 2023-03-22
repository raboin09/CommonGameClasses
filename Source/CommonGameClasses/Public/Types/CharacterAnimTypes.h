﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimTypes.generated.h"

USTRUCT(BlueprintType)
struct FAnimMontagePlayData
{
	GENERATED_BODY()

	UPROPERTY()
	UAnimMontage* MontageToPlay = nullptr;
	FName MontageSection = NAME_None;
	bool bShouldBlendOut = true;
	bool bForcePlay = false;
	bool bForceInPlace = false;
	float PlayRate = 1.f;
};