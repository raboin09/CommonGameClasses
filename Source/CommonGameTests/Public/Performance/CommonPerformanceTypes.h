// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPerformanceTypes.generated.h"

USTRUCT()
struct FFrameData
{
	GENERATED_BODY()

	UPROPERTY()
	float TimeStamp = 0.f;
	UPROPERTY()
	float FrameTime = 0.f;
};

USTRUCT()
struct FPerformanceReport
{
	GENERATED_BODY()

	UPROPERTY()
	FString BuildId = TEXT("0");

	UPROPERTY()
	FString MapName = TEXT("LVL_PerformanceTest");

	UPROPERTY()
	FDateTime TestTime = FDateTime::Now();

	UPROPERTY()
	float AverageFPS = 0.f;

	UPROPERTY()
	float MinFPS = 0.f;

	UPROPERTY()
	float MaxFPS = 0.f;

	UPROPERTY()
	float P99FPS = 0.f;

	UPROPERTY()
	TArray<FFrameData> FrameHistory;
};
