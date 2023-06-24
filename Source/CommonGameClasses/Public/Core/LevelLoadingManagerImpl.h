// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Managers/LevelLoadingManager.h"
#include "Types/CommonEventDeclarations.h"
#include "UObject/Object.h"
#include "LevelLoadingManagerImpl.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class COMMONGAMECLASSES_API ULevelLoadingManagerImpl : public UObject, public ILevelLoadingManager
{
	GENERATED_BODY()
	
public:
	ULevelLoadingManagerImpl();
	
	/////////////////////////////////
	// ILevelLoadingManager overrides
	/////////////////////////////////
	virtual void InitLoadingManager(TSoftObjectPtr<UWorld> PostProcessLevel) override;
	virtual void LoadLevelTypeWithContext(const FLevelLoadContext& LevelLoadContext) override;
	FORCEINLINE virtual FNewLevelLoadedEvent& OnNewLevelLoaded() override { return NewLevelLoadedEvent; }
	
private:
	void Internal_ApplyLoadingStateToPlayerCharacter(const bool bAddingTags) const;
	void Internal_PlayCameraFade(const bool bFadingIn) const;
	
	void Internal_LoadNewLevelWithContext(const FLevelLoadContext& LevelLoadContext);

	UFUNCTION()
	void Internal_LoadBasePostProcessLevel();
	UFUNCTION()
	void Internal_BasePostProcessLoaded();
	
	UFUNCTION()
	void Internal_UnloadPreviousLevelWithContext();
	UFUNCTION()
	void Internal_UnloadPreviousLightingLevel();
	UFUNCTION()
	void Internal_PostLevelUnload();
	UFUNCTION()
	void Internal_LoadBaseLevel();
	UFUNCTION()
	void Internal_LoadLightingLevel();
	UFUNCTION()
	void Internal_PostLevelLoad();

	FNewLevelLoadedEvent NewLevelLoadedEvent;
	
	FLevelLoadContext PreviousLevelContext;
	FLevelLoadContext CurrentLevelContext;
	
	FTimerHandle TimerHandle_WaitBeforeLevelChange;

	float TimeToWaitBeforeFadingInCamera;
	float CameraFadeLength;

	UPROPERTY()
	TSoftObjectPtr<UWorld> BasePostProcessWorld;
	UPROPERTY()
	UWorld* CurrentStreamedWorld;
	UPROPERTY()
	UWorld* PersistentWorld;
};
