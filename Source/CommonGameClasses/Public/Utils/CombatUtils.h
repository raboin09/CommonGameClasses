﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Interactable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/CombatTypes.h"
#include "Types/ResourceTypes.h"
#include "CombatUtils.generated.h"

#define OUTLINE_COLOR_RED 254
#define OUTLINE_COLOR_GREEN 252
#define OUTLINE_COLOR_GRAY 255
#define OUTLINE_COLOR_PURPLE 253

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCombatUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static float GetHitImpulseValue(EHitReactType InHit);
	static bool ShouldHitKnockback(EHitReactType InHit);
	static float GetKnockbackRecoveryTime(EHitReactType InHit);
	static bool IsBoneNameHead(const FName& InBone);
	static FName GetNearestValidBoneForImpact(FName InBone);
	static FRotator GetRotationFromComponentHit(const FHitResult& Impact);
};
