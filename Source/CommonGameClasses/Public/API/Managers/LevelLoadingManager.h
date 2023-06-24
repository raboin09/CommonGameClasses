// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Types/CommonEventDeclarations.h"
#include "LevelLoadingManager.generated.h"

USTRUCT(BlueprintType)
struct FLevelLoadContext : public FTableRowBase
{
	GENERATED_BODY()

	FLevelLoadContext()
	{
		bShouldCameraFade = true;
	}

	FLevelLoadContext(const bool bInShouldCameraFade)
	{
		bShouldCameraFade = bInShouldCameraFade;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldCameraFade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> PostProcessLevelToLoad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> BaseLevelToLoad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> LightingLevelToLoad;
};

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
