// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonAICharacter.h"
#include "ActorComponent/InteractionComponent.h"
#include "AI/CommonAIController.h"

ACommonAICharacter::ACommonAICharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	AIControllerClass = ACommonAIController::StaticClass();
}

void ACommonAICharacter::HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload)
{
	K2_HandleInteractionStarted(InteractionEventPayload);
}

void ACommonAICharacter::HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload)
{
	K2_HandleInteractionInitiated(InteractionEventPayload);
}

void ACommonAICharacter::HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload)
{
	K2_HandleMeshOutlining(InteractionOutlineEventPayload);
}
