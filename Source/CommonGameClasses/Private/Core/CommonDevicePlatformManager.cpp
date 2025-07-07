
#include "CommonLogTypes.h"
#include "Core/CommonDeviceProfileManager.h"
#include "DeviceProfiles/DeviceProfile.h"
#include "DeviceProfiles/DeviceProfileManager.h"
#include "HAL/FileManager.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/IConsoleManager.h"
#include "Types/CommonCoreTypes.h"


void UCommonDeviceProfileManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    LoadProfileSettings();
    DetectAndApplyPlatform();
}

void UCommonDeviceProfileManager::DetectAndApplyPlatform()
{
    if(IsSteamDeck())
    {
        CurrentPlatform = EPlatformType::SteamDeck;
        COMMON_PRINT_SCREEN_GREEN(TEXT("Detected Steam Deck"), 10.f)
    } else
    {
        CurrentPlatform = EPlatformType::Windows;
        COMMON_PRINT_SCREEN_GREEN(TEXT("Detected Windows"), 10.f)
    }
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
    FDeviceProfileCommands::SetProfile({ ProfileName });
    SaveProfileSettings();
    
    OnDeviceProfileChanged.Broadcast(Platform, Quality);
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

void FDeviceProfileCommands::Register()
{
    // List all available device profiles
    IConsoleManager::Get().RegisterConsoleCommand(
        TEXT("Common.Core.ListDeviceProfiles"),
        TEXT("List all available device profiles"),
        FConsoleCommandDelegate::CreateStatic(&FDeviceProfileCommands::ListProfiles)
    );

    // Set active device profile
    IConsoleManager::Get().RegisterConsoleCommand(
        TEXT("Common.Core.SetDeviceProfile"),
        TEXT("Set active device profile (Usage: Common.Core.set ProfileName)"),
        FConsoleCommandWithArgsDelegate::CreateStatic(&FDeviceProfileCommands::SetProfile)
    );

    // Show current device profile
    IConsoleManager::Get().RegisterConsoleCommand(
        TEXT("Common.Core.GetCurrentDeviceProfile"),
        TEXT("Show current active device profile"),
        FConsoleCommandDelegate::CreateStatic(&FDeviceProfileCommands::ShowCurrentProfile)
    );

    // Reload device profiles
    IConsoleManager::Get().RegisterConsoleCommand(
        TEXT("Common.Core.ReloadDeviceProfile"),
        TEXT("Reload all device profiles from config"),
        FConsoleCommandDelegate::CreateStatic(&FDeviceProfileCommands::ReloadProfiles)
    );
}

void FDeviceProfileCommands::ListProfiles()
{
    TArray<FString> Profiles = GetAvailableProfiles();
    
    UE_LOG(LogCommonGameClassesCore, Log, TEXT("Available Device Profiles:"));
    UE_LOG(LogCommonGameClassesCore, Log, TEXT("------------------------"));
    
    for (const FString& ProfileName : Profiles)
    {
        const bool bIsActive = ProfileName == UDeviceProfileManager::Get().GetActiveDeviceProfileName();
        if (bIsActive)
        {
            UE_LOG(LogCommonGameClassesCore, Log, TEXT("* %s (ACTIVE)"), *ProfileName);
        }
        else
        {
            UE_LOG(LogCommonGameClassesCore, Log, TEXT("  %s"), *ProfileName);
        }
    }
}

void FDeviceProfileCommands::SetProfile(const TArray<FString>& Args)
{
    if (Args.Num() == 0)
    {
        UE_LOG(LogCommonGameClassesCore, Warning, TEXT("Usage: Common.Core.SetDeviceProfile <ProfileName>"));
        UE_LOG(LogCommonGameClassesCore, Warning, TEXT("Available profiles:"));
        ListProfiles();
        return;
    }

    FString ProfileName = Args[0];
    UDeviceProfileManager& ProfileManager = UDeviceProfileManager::Get();
    
    if (UDeviceProfile* Profile = ProfileManager.FindProfile(ProfileName))
    {
        FString PreviousProfile = ProfileManager.GetActiveDeviceProfileName();
        ProfileManager.SetOverrideDeviceProfile(Profile);
        UE_LOG(LogCommonGameClassesCore, Log, TEXT("Changed device profile from '%s' to '%s'"), *PreviousProfile, *ProfileName);
        FString ScreenMessage = FString::Printf(TEXT("Device profile: %s"), *ProfileName);
        COMMON_PRINT_SCREEN_GREEN(ScreenMessage, 10.f)
        UE_LOG(LogCommonGameClassesCore, Log, TEXT("Current Profile Settings:"));
        UE_LOG(LogCommonGameClassesCore, Log, TEXT("%s"), *GetCurrentProfileInfo());
    }
    else
    {
        FString ScreenMessage = FString::Printf(TEXT("Device profile: %s"), *ProfileName);
        COMMON_PRINT_SCREEN_RED(ScreenMessage, 10.f)
        UE_LOG(LogCommonGameClassesCore, Error, TEXT("Profile '%s' not found!"), *ProfileName);
        UE_LOG(LogCommonGameClassesCore, Error, TEXT("Available profiles:"));
        ListProfiles();
    }
}

void FDeviceProfileCommands::ShowCurrentProfile()
{
    UDeviceProfileManager& ProfileManager = UDeviceProfileManager::Get();
    FString CurrentProfile = ProfileManager.GetActiveDeviceProfileName();
    
    UE_LOG(LogCommonGameClassesCore, Log, TEXT("Current Device Profile: %s"), *CurrentProfile);
    UE_LOG(LogCommonGameClassesCore, Log, TEXT("Settings:"));
    UE_LOG(LogCommonGameClassesCore, Log, TEXT("%s"), *GetCurrentProfileInfo());
}

void FDeviceProfileCommands::ReloadProfiles()
{
    UDeviceProfileManager& ProfileManager = UDeviceProfileManager::Get();
    FString CurrentProfile = ProfileManager.GetActiveDeviceProfileName();
    
    // Reload profiles
    ProfileManager.LoadProfiles();
    
    // Reapply current profile
    if (UDeviceProfile* Profile = ProfileManager.FindProfile(CurrentProfile))
    {
        ProfileManager.SetOverrideDeviceProfile(Profile);
        UE_LOG(LogCommonGameClassesCore, Log, TEXT("Profiles reloaded and '%s' reapplied successfully"), *CurrentProfile);
    }
    else
    {
        UE_LOG(LogCommonGameClassesCore, Warning, TEXT("Profiles reloaded but failed to reapply '%s'"), *CurrentProfile);
    }
}

FString FDeviceProfileCommands::GetCurrentProfileInfo()
{
    UDeviceProfileManager& ProfileManager = UDeviceProfileManager::Get();
    UDeviceProfile* CurrentProfile = ProfileManager.GetActiveProfile();
    
    if (!CurrentProfile)
    {
        return TEXT("No active profile!");
    }

    FString Info;
    
    // Basic info
    Info += FString::Printf(TEXT("Profile: %s\n"), *CurrentProfile->GetName());
    Info += FString::Printf(TEXT("Base Profile: %s\n"), *CurrentProfile->BaseProfileName);
    Info += FString::Printf(TEXT("Device Type: %s\n"), *CurrentProfile->DeviceType);
    
    // CVars
    Info += TEXT("\nConsole Variables:\n");
    for (const FString& CVar : CurrentProfile->CVars)
    {
        Info += FString::Printf(TEXT("  %s\n"), *CVar);
    }
    
    return Info;
}

TArray<FString> FDeviceProfileCommands::GetAvailableProfiles()
{
    TArray<FString> ProfileNames;
    UDeviceProfileManager& ProfileManager = UDeviceProfileManager::Get();
    
    const TArray<UDeviceProfile*>& Profiles = ProfileManager.Profiles;
    for (const UDeviceProfile* Profile : Profiles)
    {
        if (Profile)
        {
            ProfileNames.Add(Profile->GetName());
        }
    }
    
    ProfileNames.Sort();
    return ProfileNames;
}