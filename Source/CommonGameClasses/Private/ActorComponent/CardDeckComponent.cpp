#include "ActorComponent/CardDeckComponent.h"

void UCardDeckComponent::ShufflePileOfCards(TArray<UCardBase*>& PileToShuffle)
{
	const int32 NumCards = PileToShuffle.Num();
	for (int32 i = NumCards - 1; i > 0; i--)
	{
		if (const int32 j = FMath::RandRange(0, i); i != j)
		{
			PileToShuffle.Swap(i, j);
		}
	}
}

void UCardDeckComponent::ShuffleDiscardIntoDrawPile()
{
	ShufflePileOfCards(DiscardPile);
	DrawPile.Append(DiscardPile);
	DiscardPile.Empty();
}

UCardBase* UCardDeckComponent::DrawCardFromDrawPile()
{
	if (DrawPile.Num() <= 0)
	{
		ShuffleDiscardIntoDrawPile();
		if (DrawPile.Num() <= 0)
		{
			checkNoEntry();
			return nullptr;
		}
	}
	UCardBase* DrawnCard = DrawPile.Pop();
	return DrawnCard;
}

bool UCardDeckComponent::TryAddCardToDrawPile(UCardBase* PotentialCard)
{
	if(CanAddCardToDeck(PotentialCard))
	{
		AddCardToDrawPile(PotentialCard);
		return true;
	}
	return false;
}

UCardBase* UCardDeckComponent::GetRandomCardFromDrawPile()
{
	if(DrawPile.Num() == 0)
	{
		return TObjectPtr<UCardBase>();
	}
	return DrawPile[FMath::RandRange(0, DrawPile.Num() - 1)];
}

bool UCardDeckComponent::CanAddCardToDeck(const UCardBase* PotentialCard) const
{
	if (!IsValid(PotentialCard))
	{
		return false;
	}
	return PotentialCard->K2N_CanAddCard();
}

void UCardDeckComponent::DrawCardFromDeckToHand()
{
	if(UCardBase* DrawnCard = DrawCardFromDrawPile())
	{
		AddCardToHand(DrawnCard);
	} else
	{
		checkNoEntry();
	}
}

void UCardDeckComponent::PlayCardFromHand(int32 CardPositionInHand)
{
	if(CardPositionInHand < 0 || CardPositionInHand >= CardHand.Num())
	{
		checkNoEntry();
		return;
	}
	if(UCardBase* CardToPlay = CardHand[CardPositionInHand])
	{
		CardToPlay->PlayCard();
		MoveCardFromHandToDiscard(CardPositionInHand);
	} else
	{
		checkNoEntry();
	}
}

void UCardDeckComponent::AddCardToHand(UCardBase* NewCard)
{
	if(NewCard)
	{
		CardHand.Add(NewCard);
		NewCard->OnCardDrawn();
	}
}

void UCardDeckComponent::MoveCardFromHandToDiscard(int32 CardPositionInHand)
{
	if(CardPositionInHand < 0 || CardPositionInHand >= CardHand.Num())
	{
		checkNoEntry();
		return;
	}
	if(UCardBase* CardToMove = CardHand[CardPositionInHand])
	{
		DiscardPile.Add(CardToMove);
		CardHand.RemoveAt(CardPositionInHand);
	}
}

void UCardDeckComponent::AddCardToDrawPile(UCardBase* NewCard)
{
	DrawPile.Add(NewCard);
}
