// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonOverlapActor.h"
#include "ActorComponent/InteractionComponent.h"
#include "API/Interactable.h"
#include "Components/SphereComponent.h"
#include "CommonInteractableActor.generated.h"

class UInteractionComponent;
class URotatingMovementComponent;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonInteractableActor : public ACommonOverlapActor, public IInteractable
{
	GENERATED_BODY()

public:
	ACommonInteractableActor();

	//////////////////////////////////////
	// Interactable Overrides
	//////////////////////////////////////
	virtual void HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload) override;
	virtual void HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload) override;
	virtual void HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload) override;
	
	//////////////////////////////////////
	// ACommonOverlapActor Overrides
	//////////////////////////////////////
	FORCEINLINE virtual UShapeComponent* K2N_GetCollisionComponent_Implementation() const override { return CollisionComp; }
	
protected:	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleConsumePickup(ACharacter* ConsumingChar);
	
	virtual void PostInitializeComponents() override;	
	virtual void K2N_HandleOverlapEvent_Implementation(AActor* OtherActor, const FHitResult& HitResult) override;
	FORCEINLINE virtual UMeshComponent* K2N_GetMesh_Implementation() const override { return PickupBase; }
	
	UFUNCTION(BlueprintNativeEvent)
	bool K2N_CanPickup(ACharacter* PotentialChar);
	virtual bool K2N_CanPickup_Implementation(ACharacter* PotentialChar);
	
	virtual void ConsumePickup(ACharacter* ConsumingChar);

	UPROPERTY(EditAnywhere, Category="CUSTOM")
	USphereComponent* CollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	UStaticMeshComponent* PickupBase;
	UPROPERTY(EditAnywhere, Category="CUSTOM")
	URotatingMovementComponent* RotatingMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CUSTOM")
	UInteractionComponent* InteractionComponent;
};
