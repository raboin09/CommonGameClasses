// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define COMMON_PRINT_STRING(message) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, message);

class FCommonGameClassesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
