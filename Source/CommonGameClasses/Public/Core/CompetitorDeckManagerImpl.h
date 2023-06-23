// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Managers/CompetitorDeckManager.h"
#include "CompetitorDeckManagerImpl.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class COMMONGAMECLASSES_API UCompetitorDeckManagerImpl : public UObject, public ICompetitorDeckManager
{
	GENERATED_BODY()
	
public:
	UCompetitorDeckManagerImpl();
	
	virtual bool TryAddCardToPlayerDeck(const FLevelCard& IncomingCard) override;
	virtual const FLevelCard& GetRandomCardFromPlayerDeck() override;
	virtual FLevelCard GetLevelCardFromID(const int32 LevelCardID) const override;
	
private:
	bool CanAddCardToPlayerDeck() const;
	
	FLevelCard CurrentArenaCard;
	FCompetitorDeck PlayerDeck;
};
