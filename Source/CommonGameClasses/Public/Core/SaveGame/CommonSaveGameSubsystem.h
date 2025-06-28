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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End UGameInstanceSubsystem interface implementation

	UFUNCTION(BlueprintCallable, Category = "COMMON|Core|SaveGame")
	void WriteSaveGame();
	UFUNCTION(BlueprintCallable, Category = "COMMON|Core|SaveGame")
	void LoadSaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "COMMON|Core|SaveGame")
	void HandlePlayerStarted(ACommonPlayerController* NewPlayerController);

protected:
	FString SaveGameSlot = "";
	FOnSaveGameEvent SaveGameLoaded;
	FOnSaveGameEvent SaveGameWritten;

	UPROPERTY(Transient)
	TObjectPtr<UCommonSaveGame> CurrentSaveGame;

private:
	static TArray<TScriptInterface<ISavableActor>> GetAllSavableActors(const UWorld* World);
	static TArray<TScriptInterface<ISavableComponent>> GetAllSavableComponents(const TScriptInterface<ISavableActor>& SavableActor);

public:
	FORCEINLINE FOnSaveGameEvent& OnSaveGameLoaded() { return SaveGameLoaded; }
	FORCEINLINE FOnSaveGameEvent& OnSaveGameWritten() { return SaveGameWritten; }
	FORCEINLINE void SetSaveGameSlot(const FString& InSaveGameSlot) { if(!InSaveGameSlot.IsEmpty()) SaveGameSlot = InSaveGameSlot; }
};
 