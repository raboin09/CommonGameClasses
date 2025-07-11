// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonAbilityTypes.generated.h"

#define EFFECT_TICK_RATE .25
#define EFFECT_TICK_CYCLE_SECONDS 5
#define EFFECT_TICK_CONVERSION (1/EFFECT_TICK_RATE)
#define EFFECT_TICKS_PER_CYCLE (EFFECT_TICK_CONVERSION*EFFECT_TICK_CYCLE_SECONDS)

UENUM(BlueprintType)
enum class EAbilityTriggerType : uint8
{
	SimpleBurst,
	SimpleMontage,
	Complex
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
	None,
	InstigatorMesh,
	AbilityMesh
};

UENUM(BlueprintType)
enum class ELineTraceDirection : uint8
{
	// From player's camera (FPS/TPS standard gun)
	Camera,
	// From weapon mesh socket (FPS/TPS laser gun)
	MeshSocket,
	// From instigator's forward vector
	InstigatorForwardVector,
	// From instigator's forward vector
	AbilityMeshForwardVector,
	// Towards mouse on screen or gamepad direction (ARPG)
	InputDirection
};