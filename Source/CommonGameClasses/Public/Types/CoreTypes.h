// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCommon, Log, All);

#define CUR_CLASS_FUNC (FString(__FUNCTION__))
#define CUR_LINE  (FString::FromInt(__LINE__))
#define CUR_CLASS_FUNC_LINE (CUR_CLASS_FUNC + "(" + CUR_LINE + ")")

#define COMMON_LOG(LogVerbosity, Param1) UE_LOG(LogCommon, LogVerbosity, TEXT("%s: %s"), *CUR_CLASS_FUNC_LINE, *FString(Param1))
#define COMMON_PRINTSCREEN(Message) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,  FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ") " + Message); 

USTRUCT(BlueprintType)
struct FNewLevelLoadedPayload
{
	GENERATED_BODY()
	
	FNewLevelLoadedPayload(): NewStreamedWorld(nullptr)
	{
	}
	
	FNewLevelLoadedPayload(UWorld* InWorld): NewStreamedWorld(InWorld)
	{
	}

	UPROPERTY()
	UWorld* NewStreamedWorld;
};
DECLARE_MULTICAST_DELEGATE_OneParam(FNewLevelLoadedEvent, const FNewLevelLoadedPayload& LevelLoadedPayload);

