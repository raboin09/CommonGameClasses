// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonCoreUtils.h"

#include "ActorComponent/HealthComponent.h"
#include "Player/CommonPlayerController.h"
#include "Character/CommonPlayerCharacter.h"
#include "Core/ActorSystems/ActorTrackingSubsystem.h"
#include "Kismet/GameplayStatics.h"

ACommonPlayerController* UCommonCoreUtils::GetCommonPlayerController(const UObject* WorldContextObject)
{
	return Cast<ACommonPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

ECameraType UCommonCoreUtils::GetCurrentCameraType(const UObject* WorldContextObject)
{
	if (const ACommonPlayerController* CurrCon = GetCommonPlayerController(WorldContextObject))
	{
		return CurrCon->GetCameraType();
	}
	return ECameraType::ThirdPerson;
}

ACommonPlayerCharacter* UCommonCoreUtils::GetCommonPlayerCharacter(const UObject* WorldContextObject)
{
	if (const ACommonPlayerController* CurrCon = GetCommonPlayerController(WorldContextObject))
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

UActorTrackingSubsystem* UCommonCoreUtils::GetActorTrackingSubsystem(const UObject* WorldContextObject)
{
	if(!WorldContextObject)
	{
		return nullptr;
	}
	
	if(UActorTrackingSubsystem* ActorTrackingSubsystem = UActorTrackingSubsystem::GetSubsystemFromWorld(WorldContextObject->GetWorld()))
	{
		return ActorTrackingSubsystem;
	}
	return nullptr;
}

UInteractionComponent* UCommonCoreUtils::GetHoveredInteractionComponentByPlayerController(const UObject* WorldContextObject)
{
	if(const ACommonPlayerController* PlayerController = GetCommonPlayerController(WorldContextObject))
	{
		return PlayerController->GetCurrentHoveredInteractionComponent().Get();
	}
	return nullptr;
}

AActor* UCommonCoreUtils::GetHoveredInteractionActorByPlayerController(const UObject* WorldContextObject)
{
	if(const ACommonPlayerController* PlayerController = GetCommonPlayerController(WorldContextObject))
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

UHealthComponent* UCommonCoreUtils::GetHealthComponentFromActor(UObject* WorldContextObject)
{
	if(!WorldContextObject || !WorldContextObject->IsA(AActor::StaticClass()))
	{
		return nullptr;
	}
	
	if (const AActor* CurrChar = Cast<AActor>(WorldContextObject))
	{
		return CurrChar->FindComponentByClass<UHealthComponent>();
	}
	return nullptr;
}
