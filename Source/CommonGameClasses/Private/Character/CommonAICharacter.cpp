// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonAICharacter.h"
#include "ActorComponent/InteractionComponent.h"
#include "AI/CommonAIController.h"

ACommonAICharacter::ACommonAICharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	AIControllerClass = ACommonAIController::StaticClass();
}

void ACommonAICharacter::SwitchOutlineOnMesh(bool bShouldOutline)
{
	if(InteractionComponent)
	{
		InteractionComponent->SwitchOutlineOnAllMeshes(bShouldOutline);
	}
	K2_HandleMeshOutlining(bShouldOutline);
}

void ACommonAICharacter::InitiateInteractionWithActor(AActor* InstigatingActor, bool bStartingInteraction)
{
	K2_HandleInteractionInitiated(InstigatingActor);
	if(InteractionComponent)
	{
		InteractionComponent->StartInteraction(InstigatingActor, bStartingInteraction);
	}
}

void ACommonAICharacter::HandleInteractionStarted(const FInteractionEventPayload& InteractionEventPayload)
{
	K2_HandleInteractionStarted(InteractionEventPayload.InstigatingActor);
}