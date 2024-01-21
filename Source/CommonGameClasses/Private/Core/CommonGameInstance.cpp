// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonGameInstance.h"
#include "Core/LevelLoadingManagerImpl.h"
#include "Utils/CommonCoreUtils.h"
#include "Utils/CommonWorldUtils.h"

UCommonGameInstance::UCommonGameInstance()
{
}

void UCommonGameInstance::Init()
{
	Super::Init();
	Internal_SetupManagers();
}

void UCommonGameInstance::Internal_SetupManagers()
{
	// Create concrete impl objects to bind to each other's events
	LevelLoadingManager = UCommonCoreUtils::CreateInterfaceOfType<ULevelLoadingManagerImpl, ILevelLoadingManager>(this);
	LevelLoadingManager->OnNewLevelLoaded().AddStatic(&UCommonWorldUtils::HandleNewLevelLoadEvent);
	LevelLoadingManager->InitLoadingManager(BasePostProcessWorld);
}
