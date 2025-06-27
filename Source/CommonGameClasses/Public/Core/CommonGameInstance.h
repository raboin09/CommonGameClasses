// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CommonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable, Category="COMMON|Core|GameInstance")
	// bool SaveGame_Synchronous(const FString& SlotName, const int32 UserIndex = 0);
	// void LoadGame_Synchronous(const FString& SlotName, const int32 UserIndex = 0);
};
