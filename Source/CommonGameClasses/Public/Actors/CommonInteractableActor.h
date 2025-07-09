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

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;
	//~ End AActor Interface

	//~ Begin IInteractable Interface
	virtual void HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload) override;
	virtual void HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload) override;
	virtual void HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload) override;
	//~ End IInteractable Interface

	//~ Begin ACommonOverlapActor Interface
	virtual void HandleValidActorBeginOverlap(AActor* RequestingActor, const FHitResult& HitResult) override;
	FORCEINLINE virtual UShapeComponent* BPN_GetCollisionComponent_Implementation() const override { return CollisionComp; }
	FORCEINLINE virtual UMeshComponent* BPN_GetMesh_Implementation() const override { return PickupBase; }
	//~ End ACommonOverlapActor Interface
	
protected:	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Overlap")
	void BPI_OnMeshOutlined(const FInteractionOutlinedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Overlap")
	void BPI_OnInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Overlap")
	void BPI_OnInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Overlap")
	void BPI_OnInteractionSucceeded(AActor* RequestingActor);
	
	virtual void StartInteraction(AActor* RequestingActor);

	UPROPERTY(EditAnywhere, Category="COMMON")
	TObjectPtr<USphereComponent> CollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COMMON")
	TObjectPtr<UStaticMeshComponent> PickupBase;
	UPROPERTY(EditAnywhere, Category="COMMON")
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="COMMON")
	TObjectPtr<UInteractionComponent> InteractionComponent;
};
