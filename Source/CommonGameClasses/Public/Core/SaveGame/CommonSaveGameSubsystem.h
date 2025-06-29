// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSaveGameSubsystem.generated.h"

class ISavableComponent;
class ISavableActor;
class ACommonPlayerController;
class UCommonSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameEvent, UCommonSaveGame*, SaveObject);


/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//~ Begin UGameInstanceSubsystem interface implementation 
	//~ End UGameInstanceSubsystem interface implementation

	UFUNCTION(BlueprintCallable, Category = "COMMON|Core|SaveGame")
	void QuickSave();
	UFUNCTION(BlueprintCallable, Category = "COMMON|Core|SaveGame")
	void QuickLoad();
	UFUNCTION(BlueprintCallable, Category = "COMMON|Core|SaveGame")
	void WriteSaveGame(const FString& SlotName, const int32 UserIndex = 0);
	UFUNCTION(BlueprintCallable, Category = "COMMON|Core|SaveGame")
	void LoadSaveGame(const FString& SlotName, const int32 UserIndex = 0);
	
protected:
	FOnSaveGameEvent SaveGameLoaded;
	FOnSaveGameEvent SaveGameWritten;

private:
	static TArray<TScriptInterface<ISavableActor>> GetAllSavableActors(const UWorld* World);
	static TArray<TScriptInterface<ISavableComponent>> GetAllSavableComponents(const TScriptInterface<ISavableActor>& SavableActor);

	static UCommonSaveGame* FindOrCreateNewSaveFile(const FString& SaveGameSlot, int32 UserIdx);
	
public:
	FORCEINLINE FOnSaveGameEvent& OnSaveGameLoaded() { return SaveGameLoaded; }
	FORCEINLINE FOnSaveGameEvent& OnSaveGameWritten() { return SaveGameWritten; }
};
 