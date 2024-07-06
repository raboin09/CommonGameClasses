// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacterAnimTypes.generated.h"

USTRUCT(BlueprintType)
struct FAnimMontagePlayData
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<UAnimMontage> MontageToPlay = nullptr;
	
	FName MontageSection = NAME_None;
	bool bForcePlay = false;
	float PlayRate = 1.f;
};

UENUM(BlueprintType)
enum class ERotationMethod : uint8
{
	NONE = 0			UMETA(DisplayName = "NONE"),
	CharacterVelocity	UMETA(DisplayName = "Rotate To Character Velocity"),
	AbsoluteControlRotation	UMETA(DisplayName = "Rotate To Control Rotation"),
	SmoothedControlRotation		UMETA(DisplayName = "Smoothed Rotate To Control Rotation"),
	AngleControlRotation		UMETA(DisplayName = "Rotate To Control Rotation When Past Angle")
};

UENUM(BlueprintType)
enum class EAimOffsetClamp : uint8
{
	Nearest = 0 UMETA(DisplayName = "Nearest"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class EAimOffsets : uint8
{
	NONE = 0	UMETA(DisplayName = "NONE"),
	Aim			UMETA(DisplayName = "Aim"),
	Look		UMETA(DisplayName = "Look")
};