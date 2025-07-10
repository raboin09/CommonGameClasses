// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacterAnimTypes.generated.h"

/**
 * @struct FAnimMontagePlayData
 * @brief Encapsulates data needed to play an animation montage.
 *
 * This struct provides a convenient way to manage details about an animation montage
 * to be played, including the montage asset, specific section to start from, playback
 * rate, and whether the montage should be forcefully played.
 *
 * Key features:
 * - Holds a reference to the montage asset to be played.
 * - Allows targeting a specific section of the montage for playback.
 * - Provides a configurable playback rate.
 * - Supports forced playback when necessary.
 *
 * Intended for use in systems involving character animation components or abilities.
 *
 * @property MontageToPlay Pointer to the animation montage asset to be played.
 * @property MontageSection Specifies the section of the montage to play. Defaults to none.
 * @property bForcePlay Indicates whether the montage should be forcefully played, overriding other animations.
 * @property PlayRate Defines the playback speed for the montage. Defaults to 1.0 (normal speed).
 */
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

/**
 * @enum ERotationMethod
 * @brief Defines various methods for handling character rotation during animation.
 *
 * This enumeration is used to specify how a character's rotation should be managed
 * in relation to different movement and control inputs. It is primarily utilized
 * in character animation systems to determine how the rotation aligns with velocity,
 * control input, or specific angles.
 *
 * Options:
 * - NONE: Disables any specific rotation logic.
 * - CharacterVelocity: Aligns rotation with the character's movement direction.
 * - AbsoluteControlRotation: Forces alignment to the exact control rotation.
 * - SmoothedControlRotation: Gradually adjusts rotation to align with the control rotation.
 * - AngleControlRotation: Rotates to the control rotation when a direction threshold is exceeded.
 */
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