// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonGameClasses.h"
#include "Core/CommonDeviceProfileManager.h"

#define LOCTEXT_NAMESPACE "FCommonGameClassesModule"

void FCommonGameClassesModule::StartupModule()
{
	FDeviceProfileCommands::Register();
}

void FCommonGameClassesModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonGameClassesModule, CommonGameClasses)