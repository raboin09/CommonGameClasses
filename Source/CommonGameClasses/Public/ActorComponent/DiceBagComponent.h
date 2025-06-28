// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DiceBagComponent.generated.h"

UENUM(BlueprintType)
enum class EDiceColor: uint8
{
	White,
	Red,
	Blue,
	Green,
	Yellow,
	Black,
	Pink,
	Purple,
	Orange,
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class COMMONGAMECLASSES_API UDiceBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="COMMON|Dice")
	virtual int32 RollDice();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Dice")
	void BPI_OnDiceRolled(int32 RolledResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COMMON|Dice")
	int32 DiceSides = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COMMON|Dice")
	EDiceColor DiceColor = EDiceColor::White;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType) 
class COMMONGAMECLASSES_API UDiceBagComponent : public UObject
{
	GENERATED_BODY()
};
