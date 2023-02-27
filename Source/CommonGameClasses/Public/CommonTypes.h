// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define OUTLINE_COLOR_RED 254
#define OUTLINE_COLOR_GREEN 252
#define OUTLINE_COLOR_GRAY 255
#define OUTLINE_COLOR_PURPLE 253

UENUM(BlueprintType)
enum class EAffiliation : uint8
{
	Allies,
	Enemies,
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