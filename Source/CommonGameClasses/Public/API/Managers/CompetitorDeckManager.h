// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CoreTypes.h"

#include "CompetitorDeckManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCompetitorDeckManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API ICompetitorDeckManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual bool TryAddCardToPlayerDeck(const FLevelCard& IncomingCard) = 0;
	UFUNCTION(BlueprintCallable)
	virtual const FLevelCard& GetRandomCardFromPlayerDeck() = 0;
	UFUNCTION(BlueprintCallable)
	virtual FLevelCard GetLevelCardFromID(const int32 LevelCardID) const = 0;
};
