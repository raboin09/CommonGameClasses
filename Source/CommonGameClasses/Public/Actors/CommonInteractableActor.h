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
	FORCEINLINE virtual UShapeComponent* BPN_GetCollisionComponent_Implementation() const override { return CollisionComp; }
	
protected:	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleConsumePickup(ACharacter* ConsumingChar);
	
	virtual void PostInitializeComponents() override;	
	virtual void BPN_HandleOverlapEvent_Implementation(AActor* OtherActor, const FHitResult& HitResult) override;
	FORCEINLINE virtual UMeshComponent* BPN_GetMesh_Implementation() const override { return PickupBase; }
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Events")
	bool BPN_CanPickup(ACharacter* PotentialChar);
	virtual bool BPN_CanPickup_Implementation(ACharacter* PotentialChar);
	
	virtual void ConsumePickup(ACharacter* ConsumingChar);

	UPROPERTY(EditAnywhere, Category="CUSTOM")
	TObjectPtr<USphereComponent> CollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	TObjectPtr<UStaticMeshComponent> PickupBase;
	UPROPERTY(EditAnywhere, Category="CUSTOM")
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CUSTOM")
	TObjectPtr<UInteractionComponent> InteractionComponent;
};
