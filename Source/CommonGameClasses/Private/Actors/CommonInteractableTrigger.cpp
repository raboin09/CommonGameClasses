// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonInteractableTrigger.h"
#include "GameFramework/Character.h"

ACommonInteractableTrigger::ACommonInteractableTrigger()
{
	bDiesAfterOverlap = true;
}

bool ACommonInteractableTrigger::K2N_CanPickup_Implementation(ACharacter* PotentialChar)
{
	if(PotentialChar && PotentialChar->IsPlayerControlled())
	{
		return true;
	}
	return false;
}

void ACommonInteractableTrigger::ConsumePickup(ACharacter* ConsumingChar)
{
	K2_ConsumePickup(ConsumingChar);
}