// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonAICharacter.h"
#include "ActorComponent/BotBehaviorComponent.h"
#include "ActorComponent/InteractionComponent.h"
#include "AI/CommonAIController.h"

ACommonAICharacter::ACommonAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	BotBehaviorComponent = CreateDefaultSubobject<UBotBehaviorComponent>(TEXT("BotBehaviorComponent"));
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
