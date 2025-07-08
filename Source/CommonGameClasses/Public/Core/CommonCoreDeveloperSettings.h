// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SaveGame/CommonSaveGame.h"
#include "CommonCoreDeveloperSettings.generated.h"

class UCommonSaveGame;

namespace CommonSaveGame
{
	static FString QuickSaveSlot = TEXT("QuickSave");
}

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "COMMOM GAME Settings"))

class COMMONGAMECLASSES_API UCommonCoreDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCommonCoreDeveloperSettings()
	{
		CategoryName = TEXT("Project");
	}

	UPROPERTY(Config, EditAnywhere, Category="SaveGame", meta = (AllowAbstract = "false"))
	TSubclassOf<UCommonSaveGame> DefaultSaveGameClass = UCommonSaveGame::StaticClass();
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="SaveGame")
	FString QuickSaveSlotName = CommonSaveGame::QuickSaveSlot;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Input")
	float GamePadDeadZone = 0.1f;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Input")
	float GamePadSensitivity = 1.f;
};
