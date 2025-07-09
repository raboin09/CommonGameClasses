// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInteractableActor.h"
#include "CommonInteractableTrigger.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonInteractableTrigger : public ACommonInteractableActor
{
	GENERATED_BODY()

public:
	ACommonInteractableTrigger();
	
	FORCEINLINE virtual UMeshComponent* BPN_GetMesh_Implementation() const override { return nullptr; };
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BPI_StartTrigger(AActor* RequestingActor);
	
	virtual bool BPN_CanActorTriggerOverlap_Implementation(AActor* RequestingActor) override;
	virtual void StartInteraction(AActor* RequestingActor) override;
};