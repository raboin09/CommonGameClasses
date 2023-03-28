// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonOverlapActor.h"
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
	virtual void SwitchOutlineOnMesh(bool bShouldOutline) override;
	virtual void InitiateInteractionWithActor(AActor* InstigatingActor, bool bStartingInteraction = true) override;
	virtual void HandleInteractionStarted(const FInteractionEventPayload& InteractionEventPayload) override;
	
	//////////////////////////////////////
	// ACommonOverlapActor Overrides
	//////////////////////////////////////
	FORCEINLINE virtual UShapeComponent* GetCollisionComponent_Implementation() const override { return CollisionComp; }
	
protected:	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(bool bIsOutlining);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionStarted(AActor* InstigatingActor);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionInitiated(AActor* InstigatingActor);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleConsumePickup(ACharacter* ConsumingChar);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteraction(AActor* InstigatingActor);
	
	virtual void PostInitializeComponents() override;	
	virtual void K2_HandleOverlapEvent_Implementation(AActor* OtherActor, const FHitResult& HitResult) override;
	FORCEINLINE virtual UMeshComponent* GetMesh_Implementation() const override { return PickupBase; }
	virtual bool CanPickup(ACharacter* PotentialChar) PURE_VIRTUAL(ABaseOverlapPickup::CanPickup, return false;)
	virtual void ConsumePickup(ACharacter* ConsumingChar);

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	UStaticMeshComponent* PickupBase;
	UPROPERTY(EditAnywhere, Category="CUSTOM")
	URotatingMovementComponent* RotatingMovementComponent;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Interact")
	bool bInteractInstantly = true;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Interact", meta=(EditCondition="!bInteractInstantly", EditConditionHides))
	float InteractTime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Interact")
	FString InteractionText = "Placeholder";

private:
	UPROPERTY()
	UInteractionComponent* InteractionComponent;
};
