// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreUtils.h"
#include "CommonPlayerController.h"
#include "CommonCharacter.h"
#include "Kismet/GameplayStatics.h"

ACommonPlayerController* UCoreUtils::GetCommonPlayerController(const UObject* ContextObject)
{
	return Cast<ACommonPlayerController>(UGameplayStatics::GetPlayerController(ContextObject, 0));
}

ACommonPlayerCharacter* UCoreUtils::GetCommonPlayerCharacter(const UObject* ContextObject)
{
	if (const ACommonPlayerController* CurrCon = GetCommonPlayerController(ContextObject))
	{
		return CurrCon->GetCommonPlayerCharacter();
	}
	return nullptr;
}

bool UCoreUtils::IsObjectPlayerControlled(const UObject* Object)
{
	if(!Object)
	{
		return false;
	}

	if(const APawn* CastedPawn = Cast<APawn>(Object))
	{
		return CastedPawn->IsPlayerControlled();
	}
	return false;
}

TScriptInterface<IInteractable> UCoreUtils::GetTargetedActorByPlayerController(const UObject* ContextObject)
{
	if(const ACommonPlayerController* PlayerController = GetCommonPlayerController(ContextObject))
	{
		return PlayerController->GetTargetedActor();
	}
	return nullptr;
}
