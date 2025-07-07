
#include "CommonLogTypes.h"
#include "Core/CommonDeviceProfileManager.h"
#include "DeviceProfiles/DeviceProfileManager.h"
#include "HAL/FileManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Types/CommonCoreTypes.h"

// Add to your game module's StartupModule()
static FAutoConsoleCommand DeviceProfileCmd(
    TEXT("Common.SetDeviceQuality"),
    TEXT("Sets the device profile quality (low/medium/high)"),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
    {
        if (Args.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Usage: Common.SetDeviceQuality <low|medium|high>"));
            return;
        }

        if (UGameInstance* GameInstance = GEngine->GameViewport->GetGameInstance())
        {
            if (UCommonDeviceProfileManager* ProfileManager = 
                GameInstance->GetSubsystem<UCommonDeviceProfileManager>())
            {
                EDeviceProfileQuality Quality;
                if (Args[0].Equals(TEXT("low"), ESearchCase::IgnoreCase))
                    Quality = EDeviceProfileQuality::Low;
                else if (Args[0].Equals(TEXT("medium"), ESearchCase::IgnoreCase))
                    Quality = EDeviceProfileQuality::Medium;
                else if (Args[0].Equals(TEXT("high"), ESearchCase::IgnoreCase))
                    Quality = EDeviceProfileQuality::High;
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Invalid quality level. Use: low, medium, or high"));
                    return;
                }

                ProfileManager->SetDeviceProfile(ProfileManager->GetCurrentPlatform(), Quality);
            }
        }
    })
);

static FAutoConsoleCommand ShowProfileCmd(
    TEXT("Common.ShowDeviceQuality"),
    TEXT("Shows current device profile settings"),
    FConsoleCommandDelegate::CreateLambda([]()
    {
        if (UGameInstance* GameInstance = GEngine->GameViewport->GetGameInstance())
        {
            if (UCommonDeviceProfileManager* ProfileManager = 
                GameInstance->GetSubsystem<UCommonDeviceProfileManager>())
            {
                EPlatformType Platform = ProfileManager->GetCurrentPlatform();
                EDeviceProfileQuality Quality = ProfileManager->GetCurrentQuality();
                
                UE_LOG(LogTemp, Log, TEXT("Current Platform: %s"),
                    *UEnum::GetValueAsString(Platform));
                UE_LOG(LogTemp, Log, TEXT("Current Quality: %s"),
                    *UEnum::GetValueAsString(Quality));
            }
        }
    })
);


void UCommonDeviceProfileManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    LoadProfileSettings();
    DetectAndApplyPlatform();
}

void UCommonDeviceProfileManager::DetectAndApplyPlatform()
{
    CurrentPlatform = IsSteamDeck() ? EPlatformType::SteamDeck : EPlatformType::Windows;
    SetDeviceProfile(CurrentPlatform, CurrentQuality);
}

bool UCommonDeviceProfileManager::IsSteamDeck()
{
    FString SteamDeckValue = FPlatformMisc::GetEnvironmentVariable(TEXT("STEAM_DECK"));
    if (!SteamDeckValue.IsEmpty())
    {
        return true;
    }

    FString SteamClientUI = FPlatformMisc::GetEnvironmentVariable(TEXT("STEAM_GAMEPADUI"));
    if (!SteamClientUI.IsEmpty())
    {
        return true;
    }

    #if PLATFORM_LINUX
    // Check Steam Deck hardware file
    static const TCHAR* HardwareInfoPaths[] = {
        TEXT("/etc/hardware-info/model"),
        TEXT("/sys/devices/virtual/dmi/id/product_name")
    };

    for (const TCHAR* Path : HardwareInfoPaths)
    {
        if (FPaths::FileExists(Path))
        {
            FString Content;
            if (FFileHelper::LoadFileToString(Content, Path))
            {
                if (Content.Contains(TEXT("Jupiter")) || Content.Contains(TEXT("Steam Deck")))
                {
                    return true;
                }
            }
        }
    }
    #endif

    return false;
}

void UCommonDeviceProfileManager::SetDeviceProfile(EPlatformType Platform, EDeviceProfileQuality Quality)
{
    CurrentPlatform = Platform;
    CurrentQuality = Quality;
    
    FString ProfileName = GetProfileName(Platform, Quality);
    ApplyDeviceProfile(ProfileName);
    SaveProfileSettings();
    
    OnDeviceProfileChanged.Broadcast(Platform, Quality);
}

void UCommonDeviceProfileManager::ApplyDeviceProfile(const FString& ProfileName)
{
    UDeviceProfileManager& ProfileManager = UDeviceProfileManager::Get();
    FString ScreenMessage = FString::Printf(TEXT("Device profile: %s"), *ProfileName);
    if (UDeviceProfile* Profile = ProfileManager.FindProfile(ProfileName))
    {
        ProfileManager.SetOverrideDeviceProfile(Profile);
        UE_LOG(LogCommonGameClassesCore, Log, TEXT("Applied device profile: %s"), *ProfileName);
        COMMON_PRINT_SCREEN_GREEN(ScreenMessage, 10.f)
    }
    else
    {
        UE_LOG(LogCommonGameClassesCore, Warning, TEXT("Failed to find device profile: %s"), *ProfileName);
        COMMON_PRINT_SCREEN_RED(ScreenMessage, 10.f)
    }
}

FString UCommonDeviceProfileManager::GetProfileName(EPlatformType Platform, EDeviceProfileQuality Quality)
{
    FString PlatformStr = Platform == EPlatformType::SteamDeck ? TEXT("SteamDeck") : TEXT("Windows");
    FString QualityStr;
    
    switch (Quality)
    {
        case EDeviceProfileQuality::High:
            QualityStr = TEXT("High");
            break;
        case EDeviceProfileQuality::Medium:
            QualityStr = TEXT("Medium");
            break;
        case EDeviceProfileQuality::Low:
            QualityStr = TEXT("Low");
            break;
    }
    
    return FString::Printf(TEXT("%s_%s"), *PlatformStr, *QualityStr);
}

void UCommonDeviceProfileManager::SaveProfileSettings()
{
    GConfig->SetInt(TEXT("DeviceProfiles"), TEXT("PlatformType"), static_cast<int32>(CurrentPlatform), GGameIni);
    GConfig->SetInt(TEXT("DeviceProfiles"), TEXT("QualityLevel"), static_cast<int32>(CurrentQuality), GGameIni);
    GConfig->Flush(false, GGameIni);
}

void UCommonDeviceProfileManager::LoadProfileSettings()
{
    int32 PlatformType = 0;
    int32 QualityLevel = static_cast<int32>(EDeviceProfileQuality::Medium); // Default to Medium
    
    GConfig->GetInt(TEXT("DeviceProfiles"), TEXT("PlatformType"), PlatformType, GGameIni);
    GConfig->GetInt(TEXT("DeviceProfiles"), TEXT("QualityLevel"), QualityLevel, GGameIni);
    
    CurrentPlatform = static_cast<EPlatformType>(PlatformType);
    CurrentQuality = static_cast<EDeviceProfileQuality>(QualityLevel);
}