// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Core/SavableActor.h"
#include "GameFramework/SaveGame.h"
#include "CommonSaveGame.generated.h"

UCLASS()
class COMMONGAMECLASSES_API UCommonSaveGame : public USaveGame
{
	GENERATED_BODY()

public:	
	UPROPERTY()
	TMap<FName, FCommonActorSaveData> ActorSaveData;
};
