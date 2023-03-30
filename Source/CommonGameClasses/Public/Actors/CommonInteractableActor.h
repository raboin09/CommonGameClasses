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
	FORCEINLINE virtual UShapeComponent* GetCollisionComponent_Implementation() const override { return CollisionComp; }
	
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
	virtual void K2_HandleOverlapEvent_Implementation(AActor* OtherActor, const FHitResult& HitResult) override;
	FORCEINLINE virtual UMeshComponent* GetMesh_Implementation() const override { return PickupBase; }
	virtual bool CanPickup(ACharacter* PotentialChar) PURE_VIRTUAL(ACommonInteractableActor::CanPickup, return false;)
	virtual void ConsumePickup(ACharacter* ConsumingChar);

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	UStaticMeshComponent* PickupBase;
	UPROPERTY(EditAnywhere, Category="CUSTOM")
	URotatingMovementComponent* RotatingMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInteractionComponent* InteractionComponent;
};
