// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "API/Interactable.h"
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
	virtual void InteractWithActor(AActor* InstigatingActor) override;
	FORCEINLINE virtual EAffiliation GetAffiliation() const override { return EAffiliation::Neutral; }
	virtual FString GetInteractionText() const override { return InteractionText; }
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(bool bIsOutlining);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Eventss")
	void K2_HandleInteraction(AActor* InstigatingActor);
	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	FString InteractionText = "Placeholder";
	
private:
	UPROPERTY()
	UInteractionComponent* InteractionComponent;
};
