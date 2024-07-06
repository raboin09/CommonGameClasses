// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "UObject/Object.h"
#include "CommonAnimInstanceProxy.generated.h"

USTRUCT()
struct COMMONGAMECLASSES_API FCommonAnimationInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	friend class UCommonAnimInstance;

	FCommonAnimationInstanceProxy() = default;
	explicit FCommonAnimationInstanceProxy(UAnimInstance* AnimationInstance): FAnimInstanceProxy(AnimationInstance) { };
};
