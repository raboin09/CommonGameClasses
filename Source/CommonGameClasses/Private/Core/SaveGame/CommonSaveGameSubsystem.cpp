// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SaveGame/CommonSaveGameSubsystem.h"

#include "CommonLogTypes.h"
#include "API/Core/Savable.h"
#include "Core/CommonCoreDeveloperSettings.h"
#include "Core/SaveGame/CommonSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UCommonSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UCommonCoreDeveloperSettings* SaveGameSettings = GetDefault<UCommonCoreDeveloperSettings>();
	SaveGameSlot = SaveGameSettings->SaveSlotName;
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlot, 0))
	{
		CurrentSaveGame = Cast<UCommonSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlot, 0));
	}
	else
	{
		CurrentSaveGame = CastChecked<UCommonSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSettings->DefaultSaveGameClass));
	}
}

void UCommonSaveGameSubsystem::WriteSaveGame()
{
	TArray<AActor*> SavableActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USavable::StaticClass(), SavableActors);
	for(AActor* Actor : SavableActors)
	{
		if(!IsValid(Actor))
		{
			continue;
		}
		UE_LOGFMT(LogCommonGameClassesCore, Log, "Saving {Actor}", *Actor->GetName());
		ISavable* SavableActor = Cast<ISavable>(Actor);
		if(!SavableActor)
		{
			continue;
		}
		FCommonActorSaveData ActorSaveData = SavableActor->CreateActorSaveData();
		FMemoryWriter ActorSaveDataMemoryWriter = FMemoryWriter(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Archiver = FObjectAndNameAsStringProxyArchive(ActorSaveDataMemoryWriter, true);
		Archiver.ArIsSaveGame = true;
		Actor->Serialize(Archiver);
		CurrentSaveGame->ActorSaveData.Add(ActorSaveData.ActorUniqueId, ActorSaveData);
		UE_LOGFMT(LogCommonGameClassesCore, Log, "Saved {Actor}", *Actor->GetName());
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveGameSlot, 0);
	OnSaveGameLoaded().Broadcast(CurrentSaveGame);
}

void UCommonSaveGameSubsystem::LoadSaveGame(const FString& SlotName)
{
	SetSaveGameSlot(SlotName);
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlot, 0))
	{
		CurrentSaveGame = Cast<UCommonSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlot, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOGFMT(LogCommonGameClassesCore, Warning, "Failed to load SaveGame Data.");
			return;
		}

		UE_LOGFMT(LogCommonGameClassesCore, Log, "Loaded SaveGame Data.");

		TArray<AActor*> SavableActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USavable::StaticClass(), SavableActors);
		for(AActor* Actor : SavableActors)
		{
			if(!IsValid(Actor))
			{
				continue;
			}
			UE_LOGFMT(LogCommonGameClassesCore, Log, "Iterating {Actor}", *Actor->GetName());
			ISavable* SavableActor = Cast<ISavable>(Actor);
			if(!SavableActor)
			{
				continue;
			}
			UE_LOGFMT(LogCommonGameClassesCore, Log, "{Actor} is Savable", *Actor->GetName());
			if(!CurrentSaveGame->ActorSaveData.Contains(Actor->GetFName()))
			{
				UE_LOGFMT(LogCommonGameClassesCore, Warning, "Failed to find actor data for %s", *Actor->GetName());
				continue;
			}
			const FCommonActorSaveData& FoundData = *CurrentSaveGame->ActorSaveData.Find(Actor->GetFName());
			FMemoryReader ActorSaveDataMemoryRead = FMemoryReader(FoundData.ByteData);
			FObjectAndNameAsStringProxyArchive Archiver = FObjectAndNameAsStringProxyArchive(ActorSaveDataMemoryRead, true);
			Archiver.ArIsSaveGame = true;
			Actor->Serialize(Archiver);
			UE_LOGFMT(LogCommonGameClassesCore, Log, "Applying SaveData to {Actor}", *Actor->GetName());
			SavableActor->ApplyActorSaveData(FoundData);
		}
		OnSaveGameLoaded().Broadcast(CurrentSaveGame);
	}
	else
	{
		const UCommonCoreDeveloperSettings* SaveGameSettings = GetDefault<UCommonCoreDeveloperSettings>();
		CurrentSaveGame = CastChecked<UCommonSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSettings->DefaultSaveGameClass));
		UE_LOGFMT(LogCommonGameClassesCore, Log, "Created New SaveGame Data.");
	}
}

void UCommonSaveGameSubsystem::HandlePlayerStarted(ACommonPlayerController* NewPlayerController)
{
	
}
