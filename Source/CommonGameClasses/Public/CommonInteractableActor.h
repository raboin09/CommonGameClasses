// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "Interactable.h"
#include "CommonInteractableActor.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	FString InteractionText = "Placeholder";
	
private:
	UPROPERTY()
	class UInteractionComponent* InteractionComponent;
};
