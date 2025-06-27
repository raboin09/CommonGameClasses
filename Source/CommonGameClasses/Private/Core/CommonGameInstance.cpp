// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonGameInstance.h"

//
// bool UCommonGameInstance::SaveGame_Synchronous(const FString& SlotName, const int32 UserIndex /* = 0 */)
// {
// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saving game..."));
//
// 	TSubclassOf<UCommonSaveGame> DefaultSaveGameClass = nullptr;
// 	if (const UCommonCoreDeveloperSettings* Settings = GetDefault<UCommonCoreDeveloperSettings>())
// 	{
// 		DefaultSaveGameClass = Settings->DefaultSaveGameClass;
// 	}
//
// 	if(!DefaultSaveGameClass)
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No DefaultSaveGameClass assign in CommonDeveloperSettings!"));
// 		return false;
// 	}
// 	
// 	if (UCommonSaveGame* SaveGameInstance = Cast<UCommonSaveGame>(UGameplayStatics::CreateSaveGameObject(DefaultSaveGameClass)))
// 	{
// 		// Set data on the savegame object.
// 		return UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);
// 	}
// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("There was an issue creating save game instance!"));
// 	return false;
// }
//
// void UCommonGameInstance::LoadGame_Synchronous(const FString& SlotName, const int32 UserIndex /*= 0*/)
// {
// 	if (UCommonSaveGame* LoadedGame = Cast<UCommonSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex)))
// 	{
// 		// The operation was successful, so LoadedGame now contains the data we saved earlier. 
// 	}
// }
