// CommonDevicePlatformManager.cpp
#include "Core/CommonDevicePlatformManager.h"

#include "DeviceProfiles/DeviceProfileManager.h"
#include "HAL/FileManager.h"
#include "GenericPlatform/GenericPlatformProperties.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCoreTypes.h"

void UCommonDevicePlatformManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    // Detect platform and apply profile on initialization
    CurrentPlatform = DetectPlatform();
    SetupPlatformProfiles();
    ApplyPlatformProfile();
}

EPlatformType UCommonDevicePlatformManager::DetectPlatform()
{
    if (IsSteamDeck())
    {
        return EPlatformType::SteamDeck;
    }
    
    if (IsPS5())
    {
        return EPlatformType::PS5;
    }
    
    if (IsWindows())
    {
        return EPlatformType::Windows;
    }
    
    return EPlatformType::Unknown;
}

bool UCommonDevicePlatformManager::IsWindows()
{
#if PLATFORM_WINDOWS
    // Make sure we're not on Steam Deck running through Proton
    if (!IsSteamDeck())
    {
        return true;
    }
#endif
    
    return false;
}


bool UCommonDevicePlatformManager::IsSteamDeck()
{
    // Method 1: Check environment variable
    FString SteamDeckValue = FPlatformMisc::GetEnvironmentVariable(TEXT("STEAM_DECK"));
    if (!SteamDeckValue.IsEmpty())
    {
        return true;
    }

    // Method 2: Check Steam Deck specific hardware file
    #if PLATFORM_LINUX
    if (FPaths::FileExists(TEXT("/etc/hardware-info/model")))
    {
        FString ModelContent;
        if (FFileHelper::LoadFileToString(ModelContent, TEXT("/etc/hardware-info/model")))
        {
            return ModelContent.Contains(TEXT("Jupiter"));
        }
    }
    #endif

    return false;
}

bool UCommonDevicePlatformManager::IsPS5()
{
    #if PLATFORM_PS5
        return true;
    #else
        return false;
    #endif
}

void UCommonDevicePlatformManager::SetupPlatformProfiles()
{
    UDeviceProfileManager& DeviceProfileManager = UDeviceProfileManager::Get();
    
    // Ensure profiles exist
    if (!DeviceProfileManager.FindProfile(TEXT("WindowsProfile")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Windows profile not found!"));
    }
    
    if (!DeviceProfileManager.FindProfile(TEXT("PS5Profile")))
    {
        UE_LOG(LogTemp, Warning, TEXT("PS5 profile not found!"));
    }
    
    if (!DeviceProfileManager.FindProfile(TEXT("SteamDeck")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Steam Deck profile not found!"));
    }
}

void UCommonDevicePlatformManager::ApplyPlatformProfile()
{
    ApplyProfileForPlatform(CurrentPlatform);
}

void UCommonDevicePlatformManager::ApplyProfileForPlatform(EPlatformType Platform)
{
    UDeviceProfileManager& DeviceProfileManager = UDeviceProfileManager::Get();
    FString ProfileName;
    
    switch (Platform)
    {
        case EPlatformType::Windows:
            ProfileName = TEXT("Windows");
            break;
            
        case EPlatformType::PS5:
            ProfileName = TEXT("PS5Profile");
            break;
            
        case EPlatformType::SteamDeck:
            ProfileName = TEXT("SteamDeck");
            break;
            
        default:
            ProfileName = TEXT("DefaultProfile");
            break;
    }
    
    if (UDeviceProfile* Profile = DeviceProfileManager.FindProfile(ProfileName))
    {
        DeviceProfileManager.SetOverrideDeviceProfile(Profile);
        UE_LOG(LogCommonGameClasses, Log, TEXT("Applied profile for platform %s: %s"), 
               *UEnum::GetValueAsString(Platform), *ProfileName);
        COMMON_PRINT_SCREEN_GREEN("Applied Profile " + ProfileName, 10.f)
    }
    else
    {
        COMMON_PRINT_SCREEN_RED("Could not apply profile " + ProfileName, 10.f)
        UE_LOG(LogCommonGameClasses, Warning, TEXT("Profile not found for platform %s: %s"), 
               *UEnum::GetValueAsString(Platform), *ProfileName);
    }
}