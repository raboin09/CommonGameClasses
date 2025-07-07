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
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonGameClassesModule, CommonGameClasses)