// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonInputUtils.h"

#include "GameFramework/InputDeviceSubsystem.h"
#include "Player/CommonPlayerController.h"
#include "Utils/CommonCoreUtils.h"

bool UCommonInputUtils::IsUsingGamepad(const UObject* WorldContextObject)
{
	if (const UInputDeviceSubsystem* SubSystem = UInputDeviceSubsystem::Get())
	{
		const ACommonPlayerController* PlayerController = UCommonCoreUtils::GetCommonPlayerController(WorldContextObject);
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		const FHardwareDeviceIdentifier Hardware = SubSystem->GetMostRecentlyUsedHardwareDevice(LocalPlayer->GetPlatformUserId());
		switch(Hardware.PrimaryDeviceType)
		{
		case EHardwareDevicePrimaryType::KeyboardAndMouse:
			return false;
		case EHardwareDevicePrimaryType::Gamepad:
			return true;
		default:
			return false;
		}
	}
	return false;
}
