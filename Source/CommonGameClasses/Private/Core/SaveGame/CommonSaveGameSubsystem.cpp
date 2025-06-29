// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SaveGame/CommonSaveGameSubsystem.h"

#include "CommonLogTypes.h"
#include "API/Core/SavableActor.h"
#include "API/Core/SavableComponent.h"
#include "Core/CommonCoreDeveloperSettings.h"
#include "Core/SaveGame/CommonSaveGame.h"
#include "Core/SaveGame/CommonSaveGameTypes.h"
#include "Kismet/GameplayStatics.h"

void UCommonSaveGameSubsystem::QuickSave()
{
	const UCommonCoreDeveloperSettings* SaveGameSettings = GetDefault<UCommonCoreDeveloperSettings>();
	WriteSaveGame(SaveGameSettings->QuickSaveSlotName, 0);
}

void UCommonSaveGameSubsystem::QuickLoad()
{
	const UCommonCoreDeveloperSettings* SaveGameSettings = GetDefault<UCommonCoreDeveloperSettings>();
	LoadSaveGame(SaveGameSettings->QuickSaveSlotName, 0);
}

void UCommonSaveGameSubsystem::WriteSaveGame(const FString& SlotName, const int32 UserIndex /* = 0 */)
{
	if(SlotName.IsEmpty())
	{
		UE_LOG(LogCommonGameClassesCore, Warning, TEXT("SaveGame slot name is empty!"));
		return;
	}
	
	UCommonSaveGame* CurrentSaveGame = FindOrCreateNewSaveFile(SlotName, UserIndex);
	if (!IsValid(CurrentSaveGame))
	{
		UE_LOG(LogCommonGameClassesCore, Warning, TEXT("Failed to find/create SaveGame Data."));
		return;
	}
	
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
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
	
	for(TScriptInterface<ISavableActor> SavableActor : GetAllSavableActors(GetWorld()))
	{
		FName ActorId = FCommonActorSaveData::GetActorSaveId(SavableActor);
		SavableActor->PostActorSaved(*CurrentSaveGame->ActorSaveData.Find(ActorId));
		for(TScriptInterface<ISavableComponent> Component : GetAllSavableComponents(SavableActor))
		{
			Component->PostComponentSaved();
		}
	}
	OnSaveGameWritten().Broadcast(CurrentSaveGame);
}

void UCommonSaveGameSubsystem::LoadSaveGame(const FString& SlotName, const int32 UserIndex /* = 0 */)
{
	if(SlotName.IsEmpty())
	{
		UE_LOG(LogCommonGameClassesCore, Warning, TEXT("SaveGame slot name is empty!"));
		return;
	}
	
	UCommonSaveGame* CurrentSaveGame = FindOrCreateNewSaveFile(SlotName, UserIndex);
	if (!IsValid(CurrentSaveGame))
	{
		UE_LOG(LogCommonGameClassesCore, Warning, TEXT("Failed to find/create SaveGame Data."));
		return;
	}

	for(TScriptInterface<ISavableActor> SavableActor : GetAllSavableActors(GetWorld()))
	{
		FName ActorId = FCommonActorSaveData::GetActorSaveId(SavableActor);
		if(!CurrentSaveGame->ActorSaveData.Contains(ActorId))
		{
			UE_LOGFMT(LogCommonGameClassesCore, Warning, "Failed to find actor data for {0}", *ActorId.ToString());
			continue;
		}
		const FCommonActorSaveData& FoundData = *CurrentSaveGame->ActorSaveData.Find(ActorId);
		FMemoryReader ActorSaveDataMemoryRead = FMemoryReader(FoundData.ByteData);
		FCommonActorSaveData::LoadSerializedData(SavableActor, ActorSaveDataMemoryRead);
		UE_LOGFMT(LogCommonGameClassesCore, Log, "Applying actor save data for {0}", *ActorId.ToString());
		SavableActor->PostActorLoadedFromSave(FoundData);
		for(TScriptInterface<ISavableComponent> Component : GetAllSavableComponents(SavableActor))
		{
			Component->PostComponentLoadedFromSave();
		}
	}
	OnSaveGameLoaded().Broadcast(CurrentSaveGame);
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
		if(!IsValid(Actor))
		{
			continue;
		}
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
		if(!ActorComponent)
		{
			continue;
		}
		SavableComponentArray.Add(ActorComponent);
	}
	return SavableComponentArray;
}

UCommonSaveGame* UCommonSaveGameSubsystem::FindOrCreateNewSaveFile(const FString& SaveGameSlot, int32 UserIdx)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlot, UserIdx))
	{
		return Cast<UCommonSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlot, UserIdx));
	}
	const UCommonCoreDeveloperSettings* SaveGameSettings = GetDefault<UCommonCoreDeveloperSettings>();
	return CastChecked<UCommonSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSettings->DefaultSaveGameClass));
}

