// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonInteractableTrigger.h"
#include "GameFramework/Character.h"

ACommonInteractableTrigger::ACommonInteractableTrigger()
{
	bDiesAfterOverlap = true;
}

bool ACommonInteractableTrigger::BPN_CanPickup_Implementation(ACharacter* PotentialChar)
{
	if(PotentialChar && PotentialChar->IsPlayerControlled())
	{
		return true;
	}
	return false;
}

void ACommonInteractableTrigger::ConsumePickup(ACharacter* ConsumingChar)
{
	BPI_ConsumePickup(ConsumingChar);
}