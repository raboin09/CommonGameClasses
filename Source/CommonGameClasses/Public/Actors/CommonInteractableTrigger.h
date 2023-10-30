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
	
	FORCEINLINE virtual UMeshComponent* GetMesh_Implementation() const override { return nullptr; };
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void K2_ConsumePickup(ACharacter* ConsumingChar);
	virtual void K2_HandleEndOverlapEvent_Implementation(AActor* ExitingActor) override;
	virtual bool CanPickup(ACharacter* PotentialChar) override;
	virtual void ConsumePickup(ACharacter* ConsumingChar) override;

	UPROPERTY()
	UObject* TriggerMachine;
	UPROPERTY(EditInstanceOnly, Category="CUSTOM|Trigger")
	TSubclassOf<UObject> TriggerLogicClass;
};
