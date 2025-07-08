// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonCheatManager.h"

#include "Core/SaveGame/CommonSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UCommonCheatManager::CommonCheatQuickSave() const
{
	if(UCommonSaveGameSubsystem* SaveGameSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UCommonSaveGameSubsystem>())
	{
		SaveGameSubsystem->QuickSave();
	}
}

void UCommonCheatManager::CommonCheatQuickLoad() const
{
	if(UCommonSaveGameSubsystem* SaveGameSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UCommonSaveGameSubsystem>())
	{
		SaveGameSubsystem->QuickLoad();
	}
}
