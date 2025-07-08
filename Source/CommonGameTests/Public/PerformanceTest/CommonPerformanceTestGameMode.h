// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CommonPerformanceTestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMETESTS_API ACommonPerformanceTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACommonPerformanceTestGameMode();


	UPROPERTY(EditAnywhere, Category = "CSV Capture")
	int32 FramesToCapture = 100;
	UPROPERTY(EditAnywhere, Category = "CSV Capture")
	float WarmupTime = 5.0f;
	UPROPERTY(EditAnywhere, Category = "CSV Capture")
	bool bQuitAfterCapture = false;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool bIsDoneCapturing;
	bool bIsWarming;
	bool bIsCapturing;
	float WarmupTimer;
	int32 CapturedFrames;
	FDateTime CaptureStartTime;

	void StartCapture();
	void StopCapture();
};
