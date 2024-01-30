// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.generated.h"

#define OUTLINE_COLOR_RED 254
#define OUTLINE_COLOR_GREEN 252
#define OUTLINE_COLOR_GRAY 255
#define OUTLINE_COLOR_PURPLE 253

#define MELEE_OUTLINE_DISTANCE 500.f
#define INTERACT_OUTLINE_DISTANCE 1000.f
#define DEFAULT_OUTLINE_DISTANCE 3000.f

UENUM(BlueprintType)
enum class ECameraType : uint8
{
	FirstPerson,
	ThirdPerson,
	TopDown
};

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

#define COMMON_TRACE_INTERACTION ECC_GameTraceChannel1
#define COMMON_TRACE_ABILITY ECC_GameTraceChannel2
#define COMMON_OBJECT_TYPE_PROJECTILE ECC_GameTraceChannel3

#define COMMON_SURFACE_Default		SurfaceType_Default
#define COMMON_SURFACE_Concrete	SurfaceType1
#define COMMON_SURFACE_Dirt		SurfaceType2
#define COMMON_SURFACE_Water		SurfaceType3
#define COMMON_SURFACE_Metal		SurfaceType4
#define COMMON_SURFACE_Wood		SurfaceType5
#define COMMON_SURFACE_Grass		SurfaceType6
#define COMMON_SURFACE_Glass		SurfaceType7
#define COMMON_SURFACE_Flesh		SurfaceType8
#define COMMON_SURFACE_Plastic		SurfaceType12
#define COMMON_SURFACE_Sand		SurfaceType13
#define COMMON_SURFACE_Ice		SurfaceType14