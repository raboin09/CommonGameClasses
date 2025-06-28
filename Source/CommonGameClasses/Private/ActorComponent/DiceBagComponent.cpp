// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DiceBagComponent.h"

int32 UDiceBase::RollDice()
{
	const int32 RolledResult = FMath::RandRange(1, DiceSides);
	BPI_OnDiceRolled(RolledResult);
	return RolledResult;
}
