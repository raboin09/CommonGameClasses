// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "LockOnComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULockOnComponent();
	virtual void BeginPlay() override;
	
	void InterpToBestTargetForMeleeAttack(const TFunction<void()>& InFinishedFunction = TFunction<void()>());
	void InterpToActor(AActor* ActorToInterpTo, const TFunction<void()>& InFinishedFunction = TFunction<void()>());
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
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
	static AActor* Internal_FindBestTargetFromActors(TArray<FHitResult> PotentialHitResults);
	static FRotator Internal_GetControllerAndActorBlendedRotation(AActor* SourceActor);
	
	void Internal_StartInterpTransition();
	UFUNCTION()
	void Internal_InterpTransitionUpdate(float Alpha);
	UFUNCTION()
	void Internal_InterpTransitionFinished();

	TFunction<void()> OnFinishedFunction;
	
	FVector TargetActorLocation;
	FRotator TargetActorRotation;

	UPROPERTY(Transient)
	AActor* SelectedActor;
	UPROPERTY()
	FTimeline LockOnInterpTimeline;
};
