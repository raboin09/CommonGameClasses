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
	void BPI_ConsumePickup(ACharacter* ConsumingChar);
	
	virtual bool BPN_CanPickup_Implementation(ACharacter* PotentialChar) override;
	virtual void ConsumePickup(ACharacter* ConsumingChar) override;
};