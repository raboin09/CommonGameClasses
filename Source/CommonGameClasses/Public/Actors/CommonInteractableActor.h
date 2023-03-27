// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "API/Interactable.h"
#include "Components/TimelineComponent.h"
#include "CommonInteractableActor.generated.h"

class UInteractionComponent;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonInteractableActor : public ACommonActor, public IInteractable
{
	GENERATED_BODY()

public:
	ACommonInteractableActor();

	//////////////////////////////////////
	// Interactable Overrides
	//////////////////////////////////////
	virtual void SwitchOutlineOnMesh(bool bShouldOutline) override;
	virtual void InteractWithActor(AActor* InstigatingActor, bool bStartingInteraction = true) override;
	FORCEINLINE virtual EAffiliation GetAffiliation() const override { return EAffiliation::Neutral; }
	virtual FString GetInteractionText() const override { return InteractionText; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(bool bIsOutlining);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteraction(AActor* InstigatingActor);

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	bool bInteractInstantly = true;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults", meta=(EditCondition="!bInteractInstantly", EditConditionHides))
	float InteractTime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	FString InteractionText = "Placeholder";
	
private:
	UFUNCTION()
	void Internal_InteractionTick();
	UFUNCTION()
	void Internal_InteractionFinished();

	UPROPERTY()
	UInteractionComponent* InteractionComponent;
	UPROPERTY()
	AActor* CachedInstigatingActor;
	UPROPERTY()
	FTimeline Timeline;
};
