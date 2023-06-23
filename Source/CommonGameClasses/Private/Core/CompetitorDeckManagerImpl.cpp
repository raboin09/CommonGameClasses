// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CompetitorDeckManagerImpl.h"

UCompetitorDeckManagerImpl::UCompetitorDeckManagerImpl()
{
	
}

bool UCompetitorDeckManagerImpl::TryAddCardToPlayerDeck(const FLevelCard& IncomingCard)
{
	if(CanAddCardToPlayerDeck())
	{
		PlayerDeck.AddCardToDeck(IncomingCard);
		return true;
	}
	return false;
}

const FLevelCard& UCompetitorDeckManagerImpl::GetRandomCardFromPlayerDeck()
{
	if(PlayerDeck.CardsInDeck.Num() <= 0)
	{
		check(false)
	}
	return PlayerDeck.GetRandomCard();
}

FLevelCard UCompetitorDeckManagerImpl::GetLevelCardFromID(const int32 LevelCardID) const
{
	for(const FLevelCard& LevelCard : PlayerDeck.CardsInDeck)
	{
		if(LevelCard.CardID == LevelCardID)
		{
			return LevelCard;
		}
	}
	return FLevelCard();
}

bool UCompetitorDeckManagerImpl::CanAddCardToPlayerDeck() const
{
	return true;
}
