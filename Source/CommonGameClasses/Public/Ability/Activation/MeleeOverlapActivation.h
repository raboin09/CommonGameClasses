// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/ActivationMechanism.h"
#include "GameFramework/Actor.h"
#include "MeleeOverlapActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API AMeleeOverlapActivation : public ACommonActor, public IActivationMechanism
{
	GENERATED_BODY()

public:
	AMeleeOverlapActivation();
	virtual void BeginPlay() override;
	
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual void Deactivate() override;
	
private:
	UPROPERTY()
	UMeshComponent* MeshComponentRef;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;

public:
	FORCEINLINE virtual void SetAbilityMesh(UMeshComponent* InMeshComponent) override { MeshComponentRef = InMeshComponent; }
};
