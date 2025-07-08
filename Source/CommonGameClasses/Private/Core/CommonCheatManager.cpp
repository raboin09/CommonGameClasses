// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonCheatManager.h"

#include "ActorComponent/HealthComponent.h"
#include "Character/CommonPlayerCharacter.h"
#include "Core/SaveGame/CommonSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/CommonCoreUtils.h"

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

void UCommonCheatManager::CommonCheatTestHealthSave() const
{
	if(UHealthComponent* PlayerHealth = UCommonCoreUtils::GetPlayerCharacterHealthComponent(this))
	{
		COMMON_PRINT_SCREEN_GREEN("HEALTH PRE SAVE: " + FString::SanitizeFloat(PlayerHealth->GetCurrentHealth()), 10.f);
		PlayerHealth->TakeDamage(10.f, nullptr, FDamageHitReactEvent());
		COMMON_PRINT_SCREEN_GREEN("HEALTH POST SAVE: " + FString::SanitizeFloat(PlayerHealth->GetCurrentHealth()), 10.f);
		CommonCheatQuickSave();
		PlayerHealth->ApplyFullHeal();
		COMMON_PRINT_SCREEN_RED("HEALTH PRE LOAD: " + FString::SanitizeFloat(PlayerHealth->GetCurrentHealth()), 10.f);
		CommonCheatQuickLoad();
		COMMON_PRINT_SCREEN_RED("HEALTH POST LOAD: " + FString::SanitizeFloat(PlayerHealth->GetCurrentHealth()), 10.f);
	}
}
