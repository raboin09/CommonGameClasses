// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SaveGame/CommonSaveGameSubsystem.h"

#include "CommonLogTypes.h"
#include "API/Core/SavableActor.h"
#include "API/Core/SavableComponent.h"
#include "Core/CommonCoreDeveloperSettings.h"
#include "Core/SaveGame/CommonSaveGame.h"
#include "Core/SaveGame/CommonSaveGameTypes.h"
#include "Kismet/GameplayStatics.h"

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
	for(TScriptInterface<ISavableActor> SavableActor : GetAllSavableActors(GetWorld()))
	{
		FCommonActorSaveData ActorSaveData = FCommonActorSaveData(SavableActor);
		FMemoryWriter ActorSaveDataMemoryWriter = FMemoryWriter(ActorSaveData.ByteData);
		FCommonActorSaveData::SaveSerializedData(SavableActor, ActorSaveDataMemoryWriter);
		CurrentSaveGame->ActorSaveData.Add(ActorSaveData.ActorUniqueId, ActorSaveData);
		SavableActor->PreActorSaved();
		for(TScriptInterface<ISavableComponent> Component : GetAllSavableComponents(SavableActor))
		{
			Component->PreComponentSaved();
		}
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveGameSlot, 0);
	
	for(TScriptInterface<ISavableActor> SavableActor : GetAllSavableActors(GetWorld()))
	{
		FName ActorId = SavableActor.GetObject()->GetFName();
		SavableActor->PostActorSaved(*CurrentSaveGame->ActorSaveData.Find(ActorId));
		for(TScriptInterface<ISavableComponent> Component : GetAllSavableComponents(SavableActor))
		{
			Component->PostComponentSaved();
		}
	}
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
			UE_LOG(LogCommonGameClassesCore, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		for(TScriptInterface<ISavableActor> SavableActor : GetAllSavableActors(GetWorld()))
		{
			FName ActorId = SavableActor.GetObject()->GetFName();
			if(!CurrentSaveGame->ActorSaveData.Contains(ActorId))
			{
				UE_LOGFMT(LogCommonGameClassesCore, Warning, "Failed to find actor data for {0}", *ActorId.ToString());
				continue;
			}
			const FCommonActorSaveData& FoundData = *CurrentSaveGame->ActorSaveData.Find(ActorId);
			FMemoryReader ActorSaveDataMemoryRead = FMemoryReader(FoundData.ByteData);
			FCommonActorSaveData::LoadSerializedData(SavableActor, ActorSaveDataMemoryRead);
			SavableActor->PostActorLoadedFromSave(FoundData);
			for(TScriptInterface<ISavableComponent> Component : GetAllSavableComponents(SavableActor))
			{
				Component->PostComponentLoadedFromSave();
			}
		}
		OnSaveGameLoaded().Broadcast(CurrentSaveGame);
	}
	else
	{
		const UCommonCoreDeveloperSettings* SaveGameSettings = GetDefault<UCommonCoreDeveloperSettings>();
		CurrentSaveGame = CastChecked<UCommonSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSettings->DefaultSaveGameClass));
		UE_LOG(LogCommonGameClassesCore, Log, TEXT("Created New SaveGame Data."));
	}
}

void UCommonSaveGameSubsystem::HandlePlayerStarted(ACommonPlayerController* NewPlayerController)
{
	
}

TArray<TScriptInterface<ISavableActor>> UCommonSaveGameSubsystem::GetAllSavableActors(const UWorld* World)
{
	TArray<AActor*> SavableActors;
	UGameplayStatics::GetAllActorsWithInterface(World, USavableActor::StaticClass(), SavableActors);

	TArray<TScriptInterface<ISavableActor>> SavableActorArray;
	for(AActor* Actor : SavableActors)
	{
		SavableActorArray.Add(Actor);
	}
	return SavableActorArray;
}

TArray<TScriptInterface<ISavableComponent>> UCommonSaveGameSubsystem::GetAllSavableComponents(const TScriptInterface<ISavableActor>& SavableActor)
{
	if(!SavableActor)
	{
		return TArray<TScriptInterface<ISavableComponent>>();
	}
	const AActor* Actor = Cast<AActor>(SavableActor.GetObject());
	if(!Actor)
	{
		return TArray<TScriptInterface<ISavableComponent>>();
	}
	TArray<UActorComponent*> ActorComponents = Actor->GetComponentsByInterface(USavableComponent::StaticClass());
	TArray<TScriptInterface<ISavableComponent>> SavableComponentArray;
	for(UActorComponent* ActorComponent : ActorComponents)
	{
		SavableComponentArray.Add(ActorComponent);
	}
	return SavableComponentArray;
}
