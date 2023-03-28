// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonInteractableTrigger.h"
#include "SMUtils.h"
#include "ActorComponent//QuestManagerComponent.h"
#include "GameFramework/Character.h"

ACommonInteractableTrigger::ACommonInteractableTrigger()
{
	bDiesAfterOverlap = true;

	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ACommonInteractableTrigger::K2_HandleEndOverlapEvent_Implementation(AActor* ExitingActor)
{
	if(!TriggerMachine)
	{
		return;
	}
	
	if(ACharacter* CastedChar = Cast<ACharacter>(ExitingActor); CastedChar && CanPickup(CastedChar))
	{
		TriggerMachine->Stop();
	}

}

bool ACommonInteractableTrigger::CanPickup(ACharacter* PotentialChar)
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
	if(!TriggerLogicClass)
	{
		return;
	}
	
	if(!TriggerMachine)
	{
		TriggerMachine = USMBlueprintUtils::CreateStateMachineInstance(TriggerLogicClass, this);
	}	

	if(TriggerMachine)
	{
		TriggerMachine->Start();
	}
}