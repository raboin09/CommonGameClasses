// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "API/Interactable.h"
#include "CommonDestructibleActor.generated.h"

class UHealthComponent;
class UEffectContainerComponent;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonDestructibleActor : public ACommonActor, public IInteractable
{
	GENERATED_BODY()

public:
	ACommonDestructibleActor();
	
protected:
	virtual void BeginPlay() override;
	
	//////////////////////////////////////
	// Interactable Overrides
	//////////////////////////////////////
	virtual void HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload) override;
	virtual void HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload) override;
	virtual void HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload) override;
	
	////////////////////////////////
	// ABaseDestructibleActor
	////////////////////////////////
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleDeath(const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM")
	float DeathBuffer = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInteractionComponent> InteractionComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> DestructibleMesh;
	
private:
	UFUNCTION()
	void HandleDeathEvent(const FActorDeathEventPayload& EventPayload);
	
	UPROPERTY()
	TObjectPtr<UEffectContainerComponent> EffectContainerComponent;
};
