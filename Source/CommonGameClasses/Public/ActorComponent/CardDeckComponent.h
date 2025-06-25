#pragma once

#include "CoreMinimal.h"
#include "CardDeckComponent.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class COMMONGAMECLASSES_API UCardBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	virtual void OnCardDrawn() { K2_OnCardDrawn();}
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Cards")
	void K2_OnCardDrawn();

	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	virtual void PlayCard() { K2_OnCardPlayed();}
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Cards")
	void K2_OnCardPlayed();

	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Cards")
	bool K2N_CanAddCard() const;
	virtual bool K2N_CanAddCard_Implementation() const { return true; }
};

UCLASS(Blueprintable, BlueprintType)
class COMMONGAMECLASSES_API UCardDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	void DrawCardFromDeckToHand();
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	void PlayCardFromHand(int32 CardPositionInHand);
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	void AddCardToHand(UCardBase* NewCard);

	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	bool TryAddCardToDrawPile(UCardBase* PotentialCard);
	
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	UCardBase* DrawCardFromDrawPile();
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	UCardBase* GetRandomCardFromDrawPile();
	
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	void ShuffleDiscardIntoDrawPile();
	
#if WITH_AUTOMATION_TESTS
	void ResetPiles() { DrawPile.Empty(); CardHand.Empty(); DiscardPile.Empty(); }
#endif
	
	UFUNCTION(BlueprintCallable, Category="COMMON|Cards")
	bool CanAddCardToDeck(const UCardBase* PotentialCard) const;
	
private:
	UPROPERTY(BlueprintGetter="GetCardHand")
	TArray<UCardBase*> CardHand;
	UPROPERTY(BlueprintGetter="GetDrawPile", EditDefaultsOnly, Category="CUSTOM|Cards")
	TArray<UCardBase*> DrawPile;
	UPROPERTY(BlueprintGetter="GetDiscardPile")
	TArray<UCardBase*> DiscardPile;

private:
	void MoveCardFromHandToDiscard(int32 CardPositionInHand);
	void AddCardToDrawPile(UCardBase* NewCard);
	static void ShufflePileOfCards(TArray<UCardBase*>& PileToShuffle);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|Cards")
	FORCEINLINE TArray<UCardBase*> GetDrawPile() const { return DrawPile; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|Cards")
	FORCEINLINE TArray<UCardBase*> GetCardHand() const { return CardHand; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|Cards")
	FORCEINLINE TArray<UCardBase*> GetDiscardPile() const { return DiscardPile; }
};
