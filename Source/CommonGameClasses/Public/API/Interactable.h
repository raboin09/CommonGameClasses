// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonTypes.h"
#include "Types/EventDeclarations.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class COMMONGAMECLASSES_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void SwitchOutlineOnMesh(bool bShouldOutline) PURE_VIRTUAL(IInteractable::SwitchOutlineOnMesh, );
	virtual void InitiateInteractionWithActor(AActor* InstigatingActor, bool bStartingInteraction = true) PURE_VIRTUAL(IInteractable::InitiateInteractionWithActor, );
	// Optional callback after InteractionComponent completes it's timeline
	UFUNCTION()
	virtual void HandleInteractionStarted(const FInteractionEventPayload& InteractionEventPayload) {}
};
