// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInteractTypes.generated.h"

#define OUTLINE_COLOR_RED 254
#define OUTLINE_COLOR_GREEN 252
#define OUTLINE_COLOR_GRAY 255
#define OUTLINE_COLOR_PURPLE 253

#define MELEE_OUTLINE_DISTANCE 500.f
#define INTERACT_OUTLINE_DISTANCE 1000.f
#define DEFAULT_OUTLINE_DISTANCE 3000.f

UENUM(BlueprintType)
enum class EAffiliation : uint8
{
	None,
	Allies,
	Enemies,
	Destructible,
	InteractionActor,
	Neutral
};

UENUM()
enum class EOutlineColor : uint8
{
	Green,
	Red,
	Gray,
	Purple
};

