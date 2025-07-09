// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonInteractableTrigger.h"
#include "Utils/CommonCoreUtils.h"

ACommonInteractableTrigger::ACommonInteractableTrigger()
{
	bDiesAfterOverlap = true;
}

bool ACommonInteractableTrigger::BPN_CanActorTriggerOverlap_Implementation(AActor* RequestingActor)
{
	if(RequestingActor && UCommonCoreUtils::IsObjectPlayerControlled(RequestingActor))
	{
		return Super::BPN_CanActorTriggerOverlap_Implementation(RequestingActor);
	}
	return false;
}

void ACommonInteractableTrigger::StartInteraction(AActor* RequestingActor)
{
	BPI_StartTrigger(RequestingActor);
}