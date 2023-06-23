// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/EventDeclarations.h"
#include "MatchmakingManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UMatchmakingManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IMatchmakingManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual bool TryStartMatchWithLevelCardID(const int32 LevelCardID) = 0;
	UFUNCTION()
	virtual void HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload) = 0;
};
