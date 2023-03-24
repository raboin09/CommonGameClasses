﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

class UTimelineComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULockOnComponent();
	virtual void BeginPlay() override;
	
	void InterpToBestTargetForMeleeAttack(TFunction<void()> InFinishedFunction = TFunction<void()>());
	void InterpToActor(AActor* ActorToInterpTo, TFunction<void()> InFinishedFunction = TFunction<void()>());
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UCurveFloat* LockOnTransitionCurve;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FVector TraceOffset = FVector(0.f, 0.f, 50.f);
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float NumCones = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float ConeTraceArcWidth = 55.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float SweepRadius = 55.f;		
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float ArcDistance = 300.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bUseControllerRotation;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bDrawDebug;
	
private:
	AActor* Internal_TraceForTarget() const;
	AActor* Internal_FindBestTargetFromActors(TArray<FHitResult> PotentialHitResults) const;
	FRotator Internal_GetControllerAndActorBlendedRotation(AActor* SourceActor) const;
	
	void Internal_StartInterpTransition();
	UFUNCTION()
	void Internal_CoverTransitionUpdate(float Alpha);
	UFUNCTION()
	void Internal_CoverTransitionFinished();

	TFunction<void()> OnFinishedFunction;
	
	FVector TargetActorLocation;
	FRotator TargetActorRotation;

	UPROPERTY(Transient)
	AActor* SelectedActor;
	UPROPERTY()
	UTimelineComponent* LockOnInterpTimeline;
};