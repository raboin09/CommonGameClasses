#include "Types/CoreTypes.h"

DEFINE_LOG_CATEGORY(LogCommonGameClasses)

void FCompetitorDeck::AddCardToDeck(FLevelCard IncomingCard)
{
	IncomingCard.CardID = ++LastIndexAssigned;
	CardsInDeck.Add(IncomingCard);
}

const FLevelCard& FCompetitorDeck::GetRandomCard()
{
	const int32 NumElementsInArray = CardsInDeck.Num();
	check(NumElementsInArray > 0);
	const int32 HighestIndexInArray = NumElementsInArray - 1;
	return CardsInDeck[FMath::RandRange(0, HighestIndexInArray)];
}