// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonGameInstance.h"
#include "Core/CompetitorDeckManagerImpl.h"
#include "Core/LevelLoadingManagerImpl.h"
#include "Core/MatchmakingManagerImpl.h"
#include "Utils/CoreUtils.h"
#include "Utils/WorldUtils.h"

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
	LevelLoadingManager = UCoreUtils::CreateInterfaceOfType<ULevelLoadingManagerImpl, ILevelLoadingManager>(this);
	MatchmakingManager = UCoreUtils::CreateInterfaceOfType<UMatchmakingManagerImpl, IMatchmakingManager>(this);
	CompetitorDeckManager = UCoreUtils::CreateInterfaceOfType<UCompetitorDeckManagerImpl, ICompetitorDeckManager>(this);

	const FName HandeLevelLoadFunctionName = "HandleNewLevelLoadEvent";	
	LevelLoadingManager->OnNewLevelLoaded().AddUFunction(MatchmakingManager.GetObject(), HandeLevelLoadFunctionName);
	LevelLoadingManager->OnNewLevelLoaded().AddStatic(&UWorldUtils::HandleNewLevelLoadEvent);
	LevelLoadingManager->InitLoadingManager(BasePostProcessWorld);
}
