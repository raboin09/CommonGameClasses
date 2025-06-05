// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonCoreUtils.h"

#include "ActorComponent/HealthComponent.h"
#include "Player/CommonPlayerController.h"
#include "Character/CommonPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/ActorTrackingSubsystem.h"
#include "Types/CommonCoreTypes.h"

ACommonPlayerController* UCommonCoreUtils::GetCommonPlayerController(const UObject* ContextObject)
{
	return Cast<ACommonPlayerController>(UGameplayStatics::GetPlayerController(ContextObject, 0));
}

ACommonPlayerCharacter* UCommonCoreUtils::GetCommonPlayerCharacter(const UObject* ContextObject)
{
	if (const ACommonPlayerController* CurrCon = GetCommonPlayerController(ContextObject))
	{
		return CurrCon->GetCommonPlayerCharacter().Get();
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

UInteractionComponent* UCommonCoreUtils::GetHoveredInteractionComponentByPlayerController(const UObject* ContextObject)
{
	if(const ACommonPlayerController* PlayerController = GetCommonPlayerController(ContextObject))
	{
		return PlayerController->GetCurrentHoveredInteractionComponent().Get();
	}
	return nullptr;
}

AActor* UCommonCoreUtils::GetHoveredInteractionActorByPlayerController(const UObject* ContextObject)
{
	if(const ACommonPlayerController* PlayerController = GetCommonPlayerController(ContextObject))
	{
		return PlayerController->GetCurrentHoveredInteractionActor().Get();
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
