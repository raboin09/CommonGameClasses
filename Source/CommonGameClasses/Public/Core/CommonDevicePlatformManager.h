// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonDevicePlatformManager.generated.h"

UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	Windows     UMETA(DisplayName = "Windows PC"),
	PS5         UMETA(DisplayName = "PlayStation 5"),
	SteamDeck   UMETA(DisplayName = "Steam Deck"),
	Unknown     UMETA(DisplayName = "Unknown Platform")
};


UCLASS()
class COMMONGAMECLASSES_API UCommonDevicePlatformManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable, Category = "COMMON|Core")
	void ApplyPlatformProfile();

	UFUNCTION(BlueprintPure, Category = "COMMON|Core")
	static EPlatformType DetectPlatform();
	UFUNCTION(BlueprintPure, Category = "COMMON|Core")
	static bool IsSteamDeck();
	UFUNCTION(BlueprintPure, Category = "COMMON|Core")
	static bool IsPS5();
	UFUNCTION(BlueprintPure, Category = "COMMON|Core")
	static bool IsWindows();

private:
	static void SetupPlatformProfiles();
	static void ApplyProfileForPlatform(EPlatformType Platform);
	EPlatformType CurrentPlatform;

};
