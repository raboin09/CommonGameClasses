// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CoreUtils.h"

#include "ActorComponent/HealthComponent.h"
#include "Player/CommonPlayerController.h"
#include "Character/CommonCharacter.h"
#include "Character/CommonPlayerCharacter.h"
#include "Core/CommonGameInstance.h"
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

UInteractionComponent* UCoreUtils::GetHoveredInteractionComponentByPlayerController(const UObject* ContextObject)
{
	if(const ACommonPlayerController* PlayerController = GetCommonPlayerController(ContextObject))
	{
		return PlayerController->GetCurrentHoveredInteractionComponent();
	}
	return nullptr;
}

UHealthComponent* UCoreUtils::GetPlayerCharacterHealthComponent(const UObject* WorldContextObject)
{
	if(!WorldContextObject){
		return nullptr;
	}
	return GetHealthComponentFromActor(GetCommonPlayerCharacter(WorldContextObject));
}

UHealthComponent* UCoreUtils::GetHealthComponentFromActor(UObject* ContextObject)
{
	if(!ContextObject || !ContextObject->IsA(AActor::StaticClass()))
	{
		return nullptr;
	}
	
	if (const AActor* CurrChar = Cast<AActor>(ContextObject))
	{
		return CurrChar->FindComponentByClass<UHealthComponent>();
	}
	return nullptr;
}

UQuestManagerComponent* UCoreUtils::GetQuestManager(const UObject* ContextObject)
{
	if (const ACommonPlayerController* BaseCon = GetCommonPlayerController(ContextObject))
	{
		return BaseCon->GetQuestManager();
	}
	return nullptr;
}

UCommonGameInstance* UCoreUtils::GetCommonGameInstance(const UObject* ContextObject)
{
	UGameInstance* GameInst = UGameplayStatics::GetGameInstance(ContextObject);
	if(!GameInst)
	{
		COMMON_LOG(Error, "GameplayStatics error getting GameInstance")
	}
	if(UCommonGameInstance* CastedGameInst = Cast<UCommonGameInstance>(GameInst))
	{
		return CastedGameInst;
	}
	COMMON_LOG(Error, "CoreUtils error getting BaseGameInstance")
	return nullptr;
}

TScriptInterface<ICompetitorDeckManager> UCoreUtils::GetCompetitorDeckManager(const UObject* ContextObject)
{
	if(const UCommonGameInstance* GameInst = GetCommonGameInstance(ContextObject))
	{
		return GameInst->GetCompetitorDeckManager();
	}
	return nullptr;
}

TScriptInterface<ILevelLoadingManager> UCoreUtils::GetLevelLoadingManager(const UObject* ContextObject)
{
	if(const UCommonGameInstance* GameInst = GetCommonGameInstance(ContextObject))
	{
		return GameInst->GetLevelLoadingManager();
	}
	COMMON_LOG(Error, "CoreUtils error getting Loading Manager")
	return nullptr;
}