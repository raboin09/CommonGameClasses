// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MatchmakingManagerImpl.h"

#include "API/Managers/CompetitorDeckManager.h"
#include "API/Managers/LevelLoadingManager.h"
#include "Character/CommonPlayerCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Types/CoreTypes.h"
#include "Utils/CoreUtils.h"
#include "Utils/WorldUtils.h"

bool UMatchmakingManagerImpl::TryStartMatchWithLevelCardID(const int32 LevelCardID)
{
	if(const TScriptInterface<ICompetitorDeckManager> DeckManager = UCoreUtils::GetCompetitorDeckManager(this))
	{
		const FLevelCard& LevelCard = DeckManager->GetLevelCardFromID(LevelCardID);
		const FLevelLoadContext* LevelLoadContextPtr = LevelCard.LevelLoadContextRow.GetRow<FLevelLoadContext>("LevelLoadContext");
		check(LevelLoadContextPtr)
		if(LevelLoadContextPtr)
		{
			const FLevelLoadContext& LevelLoadContext = *LevelLoadContextPtr;
			if(const TScriptInterface<ILevelLoadingManager> LevelLoadingManager = UCoreUtils::GetLevelLoadingManager(this))
			{
				LevelLoadingManager->LoadLevelTypeWithContext(LevelLoadContext);
			}
		}
	}
	return true;
}

void UMatchmakingManagerImpl::HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload)
{
	if(!NewLevelLoadedPayload.NewStreamedWorld)
	{
		return;
	}

	TArray<AActor*> PlayerStarts = UWorldUtils::GetAllActorsOfClassInCurrentStreamedWorld(APlayerStart::StaticClass());
	if(PlayerStarts.Num() <= 0)
	{
		return;
	}
	
	if(ACommonPlayerCharacter* BasePlayerCharacter = UCoreUtils::GetCommonPlayerCharacter(this))
	{
		BasePlayerCharacter->SetActorTransform(PlayerStarts[0]->GetTransform());
	}
}
