// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Managers/LevelLoadingManager.h"
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

protected:
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UWorld> BasePostProcessWorld;
	
private:
	UPROPERTY()
	TScriptInterface<ILevelLoadingManager> LevelLoadingManager;
	
	void Internal_SetupManagers();
};
