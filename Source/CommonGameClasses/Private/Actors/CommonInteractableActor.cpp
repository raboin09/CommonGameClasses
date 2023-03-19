#include "Actors/CommonInteractableActor.h"

#include "Character/InteractionComponent.h"

ACommonInteractableActor::ACommonInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void ACommonInteractableActor::SwitchOutlineOnMesh(bool bShouldOutline)
{
	if(InteractionComponent)
	{
		InteractionComponent->SwitchOutlineOnAllMeshes(bShouldOutline);
	}
	K2_HandleMeshOutlining(bShouldOutline);
}

void ACommonInteractableActor::InteractWithActor(AActor* InstigatingActor)
{
	K2_HandleInteraction(InstigatingActor);
}