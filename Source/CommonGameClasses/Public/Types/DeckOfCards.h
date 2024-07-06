// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template<typename InCardType>
class TDeckOfCards
{
	typedef InCardType Card;

	struct TCardEntry
	{
		TCardEntry(Card InCard) : Card(InCard)
		{
			CardGuid = FGuid::NewGuid(); 
		}
		
		FGuid CardGuid;
		Card Card;
	};
	
public:
	FORCEINLINE TDeckOfCards() {}
	
	FORCEINLINE bool TryAddCardToDeck(Card IncomingCard)
	{
		if(CanAddCardToDeck())
		{
			AddCardToDeck(IncomingCard);
			return true;
		}
		return false;
	}
	
	FORCEINLINE Card GetRandomCard()
	{
		if(CardsInDeck.Num() == 0)
		{
			return Card();
		}
		return CardsInDeck[FMath::RandRange(0, CardsInDeck.Num() - 1)].Card;
	}

	FORCEINLINE TArray<FGuid> GetAllCardHashes() const
	{
		TArray<FGuid> ListOfIndices;
		for(TCardEntry CardEntry : CardsInDeck)
		{
			ListOfIndices.Add(CardEntry.CardGuid);
		}
		return ListOfIndices;
	}

	FORCEINLINE const TArray<Card>& GetAllCards() const
	{
		TArray<Card> ListOfCards;
		for(TCardEntry CardEntry : CardsInDeck)
		{
			ListOfCards.Add(CardEntry.Card);
		}
		return ListOfCards;
	}

private:

	FORCEINLINE void AddCardToDeck(const Card& IncomingCard)
	{
		CardsInDeck.Add(TCardEntry(IncomingCard));
	}
	
	FORCEINLINE bool CanAddCardToDeck(const Card& PotentialCard) const
	{
		return true;
	}

protected:
	TArray<TCardEntry> CardsInDeck;
};
