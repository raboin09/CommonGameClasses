// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CoreTypes.h"
#include "Types/EventDeclarations.h"
#include "LevelLoadingManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class ULevelLoadingManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API ILevelLoadingManager
{
	GENERATED_BODY()

public:
	virtual void InitLoadingManager(TSoftObjectPtr<UWorld> PostProcessLevel) = 0;
	virtual FNewLevelLoadedEvent& OnNewLevelLoaded() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void LoadLevelTypeWithContext(const FLevelLoadContext& LevelLoadContext) = 0;
};
