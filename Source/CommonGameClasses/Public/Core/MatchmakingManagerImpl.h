// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Managers/MatchmakingManager.h"
#include "UObject/Object.h"
#include "MatchmakingManagerImpl.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COMMONGAMECLASSES_API UMatchmakingManagerImpl : public UObject, public IMatchmakingManager
{
	GENERATED_BODY()
	
public:
	virtual bool TryStartMatchWithLevelCardID(const int32 LevelCardID) override;
	UFUNCTION()
	virtual void HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload) override;
};
