﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

// This interface binds the actor/character to the UInteractionComponent. The UInteractionComponent is used to detect outline switches, interaction events, etc and then it broadcasts
// those events to the IInteractable owner.
class COMMONGAMECLASSES_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload) PURE_VIRTUAL(IInteractable::HandleMeshOutlining, )
	UFUNCTION()
	virtual void HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload) PURE_VIRTUAL(IInteractable::HandleInteractionStarted, )
	UFUNCTION()
	virtual void HandleInteractionInitiated(const FInteractionInitiatedEventPayload InitiatedEventPayload) PURE_VIRTUAL(IInteractable::HandleInteractionInitiated, )
};
