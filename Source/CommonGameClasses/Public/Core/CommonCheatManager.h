// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "CommonCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonCheatManager : public UCheatManager
{
	GENERATED_BODY()

protected:
	UFUNCTION(Exec, Category = "COMMON|Cheats")
	void CommonCheatQuickSave() const;
	UFUNCTION(Exec, Category = "COMMON|Cheats")
	void CommonCheatQuickLoad() const;
};
