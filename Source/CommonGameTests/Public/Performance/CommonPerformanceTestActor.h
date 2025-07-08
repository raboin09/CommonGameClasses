// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "CommonPerformanceTypes.h"
#include "CommonPerformanceTestActor.generated.h"

UCLASS()
class COMMONGAMETESTS_API ACommonPerformanceTestActor : public AFunctionalTest

{
	GENERATED_BODY()

public:
	ACommonPerformanceTestActor();

	virtual void PrepareTest() override;
	virtual void StartTest() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Performance Test")
	float TestDuration;
	UPROPERTY(EditAnywhere, Category = "Performance Test")
	float WarmupDuration;

	bool bFinishedTest = false;

private:
	float ElapsedTime;
	bool bIsWarming;
	FPerformanceReport Report;
    
	void CollectFrameData();
	void ProcessResults();
	void ExportResults() const;
	void ReportToTeamCity() const;
};
