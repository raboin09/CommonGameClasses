// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Managers/CompetitorDeckManager.h"
#include "API/Managers/LevelLoadingManager.h"
#include "API/Managers/MatchmakingManager.h"
#include "CommonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonGameInstance : public UGameInstance
{
	
public:
	GENERATED_BODY()
	
	UCommonGameInstance();
	
	////////////////////////////
	// BaseGameInstance code
	////////////////////////////
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TScriptInterface<ILevelLoadingManager> GetLevelLoadingManager() const { return LevelLoadingManager; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TScriptInterface<ICompetitorDeckManager> GetCompetitorDeckManager() const { return CompetitorDeckManager; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TScriptInterface<IMatchmakingManager> GetMatchmakingManager() const { return MatchmakingManager; }

protected:
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UWorld> BasePostProcessWorld;
	
private:
	UPROPERTY()
	TScriptInterface<ILevelLoadingManager> LevelLoadingManager;	
	UPROPERTY()
	TScriptInterface<ICompetitorDeckManager> CompetitorDeckManager;
	UPROPERTY()
	TScriptInterface<IMatchmakingManager> MatchmakingManager;
	
	void Internal_SetupManagers();
};
