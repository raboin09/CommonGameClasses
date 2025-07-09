// PlatformProfileManager.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonDeviceProfileManager.generated.h"

UENUM(BlueprintType)
enum class EDeviceProfileQuality : uint8
{
	Low,
	Medium,
	High,
	Epic
};

UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	Windows,
	SteamDeck,
	Other
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeviceProfileChanged, EPlatformType, Platform, EDeviceProfileQuality, Quality);

UCLASS()
class COMMONGAMECLASSES_API UCommonDeviceProfileManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
	UFUNCTION(BlueprintCallable, Category = "COMMON|Platform Profile")
	void DetectAndApplyPlatform();
	UFUNCTION(BlueprintCallable, Category = "COMMON|Platform Profile")
	void SetDeviceProfile(EPlatformType Platform, EDeviceProfileQuality Quality);
    
	UFUNCTION(BlueprintPure, Category = "COMMON|Platform Profile")
	EPlatformType GetCurrentPlatform() const { return CurrentPlatform; }
	UFUNCTION(BlueprintPure, Category = "COMMON|Platform Profile")
	EDeviceProfileQuality GetCurrentQuality() const { return CurrentQuality; }
	UFUNCTION(BlueprintPure, Category = "COMMON|Platform Profile")
	static bool IsSteamDeck();
    
	UPROPERTY(BlueprintAssignable, Category = "COMMON On Device Profile Changed")
	FOnDeviceProfileChanged OnDeviceProfileChanged;

private:
	static FString GetProfileName(EPlatformType Platform, EDeviceProfileQuality Quality);
	void SaveProfileSettings();
	void LoadProfileSettings();
    
	EPlatformType CurrentPlatform;
	EDeviceProfileQuality CurrentQuality;
};

class FDeviceProfileCommands
{
public:
	static void Register();
	static void SetProfile(const TArray<FString>& Args);
	
private:
	static void ListProfiles();
	static void ShowCurrentProfile();
	static void ReloadProfiles();
    
	static FString GetCurrentProfileInfo();
	static TArray<FString> GetAvailableProfiles();
};
