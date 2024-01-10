// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonCoreUtils.h"

#include "ActorComponent/HealthComponent.h"
#include "Player/CommonPlayerController.h"
#include "Character/CommonPlayerCharacter.h"
#include "Core/CommonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Types/CommonCoreTypes.h"

ACommonPlayerController* UCommonCoreUtils::GetCommonPlayerController(const UObject* ContextObject)
{
	return Cast<ACommonPlayerController>(UGameplayStatics::GetPlayerController(ContextObject, 0));
}

ACommonPlayerCharacter* UCommonCoreUtils::GetCommonPlayerCharacter(const UObject* ContextObject)
{
	if (const ACommonPlayerController* CurrCon = GetCommonPlayerController(ContextObject))
	{
		return CurrCon->GetCommonPlayerCharacter();
	}
	return nullptr;
}

bool UCommonCoreUtils::IsObjectPlayerControlled(const UObject* Object)
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

UActorTrackingSubsystem* UCommonCoreUtils::GetActorTrackingSubsystem(const UObject* ContextObject)
{
	if(!ContextObject)
	{
		return nullptr;
	}
	
	if(UActorTrackingSubsystem* ActorTrackingSubsystem = UActorTrackingSubsystem::GetSubsystemFromWorld(ContextObject->GetWorld()))
	{
		return ActorTrackingSubsystem;
	}
	return nullptr;
}

ACommonGameMode* UCommonCoreUtils::GetCommonGameMode(const UObject* ContextObject)
{
	return Cast<ACommonGameMode>(UGameplayStatics::GetGameMode(ContextObject));
}

UInteractionComponent* UCommonCoreUtils::GetHoveredInteractionComponentByPlayerController(const UObject* ContextObject)
{
	if(const ACommonPlayerController* PlayerController = GetCommonPlayerController(ContextObject))
	{
		return PlayerController->GetCurrentHoveredInteractionComponent();
	}
	return nullptr;
}

UHealthComponent* UCommonCoreUtils::GetPlayerCharacterHealthComponent(const UObject* WorldContextObject)
{
	if(!WorldContextObject){
		return nullptr;
	}
	return GetHealthComponentFromActor(GetCommonPlayerCharacter(WorldContextObject));
}

UHealthComponent* UCommonCoreUtils::GetHealthComponentFromActor(UObject* ContextObject)
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

UQuestManagerComponent* UCommonCoreUtils::GetQuestManager(const UObject* ContextObject)
{
	if (const ACommonPlayerController* BaseCon = GetCommonPlayerController(ContextObject))
	{
		return BaseCon->GetQuestManager();
	}
	return nullptr;
}

UCommonGameInstance* UCommonCoreUtils::GetCommonGameInstance(const UObject* ContextObject)
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

TScriptInterface<ILevelLoadingManager> UCommonCoreUtils::GetLevelLoadingManager(const UObject* ContextObject)
{
	if(const UCommonGameInstance* GameInst = GetCommonGameInstance(ContextObject))
	{
		return GameInst->GetLevelLoadingManager();
	}
	COMMON_LOG(Error, "CoreUtils error getting Loading Manager")
	return nullptr;
}