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
	
	void InterpToBestTargetForMeleeAttack();
	void InterpToActor(TWeakObjectPtr<AActor> ActorToInterpTo);
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float LockOnSlideDuration = .25f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FVector TraceOffset = FVector(0.f, 0.f, 50.f);
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float NumCones = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float ConeTraceArcWidth = 55.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	float SweepRadius = 55.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bUseControllerRotation = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bDrawDebug = false;
	
private:
	TWeakObjectPtr<AActor> Internal_TraceForTarget() const;
	static TWeakObjectPtr<AActor> Internal_FindBestTargetFromActors(TArray<FHitResult> PotentialHitResults);
	static FRotator Internal_GetControllerAndActorBlendedRotation(TWeakObjectPtr<AActor> SourceActor);
	// Check if not nullptr and if in distance
	bool Internal_IsActorValidTarget(TWeakObjectPtr<AActor> InActor) const;
	
	void Internal_StartInterpTransition();
	UFUNCTION()
	void Internal_InterpTransitionUpdate();
	UFUNCTION()
	void Internal_InterpTransitionFinished();
	
	FVector TargetActorLocation;
	FRotator TargetActorRotation;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> SelectedActor;
	UPROPERTY()
	FTimeline LockOnInterpTimeline;
};
