// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FResourceCostData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float ResourceCost;
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