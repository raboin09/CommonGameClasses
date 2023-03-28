// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "API/Interactable.h"
#include "GameFramework/Actor.h"
#include "CommonDestructibleActor.generated.h"

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
	virtual void SwitchOutlineOnMesh(bool bShouldOutline) override;
	virtual void InitiateInteractionWithActor(AActor* InstigatingActor, bool bStartingInteraction = true) override { }
	virtual void HandleInteractionStarted(const FInteractionEventPayload& InteractionEventPayload) override { }
	
	////////////////////////////////
	// ABaseDestructibleActor
	////////////////////////////////
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleDeath(const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(bool bIsOutlining);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionStarted(AActor* InstigatingActor);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionInitiated(AActor* InstigatingActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genestealer|Defaults")
	float DeathBuffer = 5.f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* DestructibleMesh;
	
private:
	UFUNCTION()
	void HandleDeathEvent(const FActorDeathEventPayload& EventPayload);
	
	UPROPERTY()
	class UEffectContainerComponent* EffectContainerComponent;
	UPROPERTY()
	class UInteractionComponent* InteractionComponent;
};
