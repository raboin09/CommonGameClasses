// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AbilityTypes.generated.h"

#define EFFECT_TICK_RATE .25
#define EFFECT_TICK_CYCLE_SECONDS 5
#define EFFECT_TICK_CONVERSION (1/EFFECT_TICK_RATE)
#define EFFECT_TICKS_PER_CYCLE (EFFECT_TICK_CONVERSION*EFFECT_TICK_CYCLE_SECONDS)

USTRUCT(BlueprintType)
struct FResourceCostData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float ResourceCost = 0.f;
};

UENUM(BlueprintType)
enum class EResourceContainerLocation : uint8
{
	Instigator,
	InstigatorComponent,
	PlayerController,
	PlayerControllerComponent,
	Ability,
	AbilityComponent,
	PlayerState,
	PlayerStateComponent
};

UENUM(BlueprintType)
enum class EMeshType : uint8
{
	InstigatorMesh,
	AbilityMesh
};

UENUM(BlueprintType)
enum class ELineTraceDirection : uint8
{
	// From player's camera (FPS/TPS standard gun)
	Camera,
	// From weapon mesh (FPS/TPS laser gun)
	Mesh,
	// Towards mouse direction on screen (ARPG)
	Mouse
};