// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FLevelCard : public FTableRowBase
{
	GENERATED_BODY()

	FLevelCard()
	{
		CardID = -1;
	}

	int32 CardID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RowType=LevelLoadContext))
	FDataTableRowHandle LevelLoadContextRow;
};


USTRUCT(BlueprintType)
struct FCompetitorDeck
{
	GENERATED_BODY()

	FCompetitorDeck()
	{
		LastIndexAssigned = 0;
	}

	void AddCardToDeck(FLevelCard IncomingCard);

	const FLevelCard& GetRandomCard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLevelCard> CardsInDeck;

	int32 LastIndexAssigned;
};

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

DECLARE_LOG_CATEGORY_EXTERN(LogCommonGameClasses, Log, All);

#define CUR_CLASS_FUNC (FString(__FUNCTION__))
#define CUR_LINE  (FString::FromInt(__LINE__))
#define CUR_CLASS_FUNC_LINE (CUR_CLASS_FUNC + "(" + CUR_LINE + ")")

#define COMMON_LOG(LogVerbosity, Param1) UE_LOG(LogCommonGameClasses, LogVerbosity, TEXT("%s: %s"), *CUR_CLASS_FUNC_LINE, *FString(Param1))
#define COMMON_PRINTSCREEN(Message) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,  FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ")\n" + Message); 

