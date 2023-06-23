// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/LevelLoadingManagerImpl.h"

#include "ActorComponent/GameplayTagComponent.h"
#include "Character/CommonPlayerCharacter.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "Types/TagTypes.h"
#include "Utils/CoreUtils.h"
#include "Utils/WorldUtils.h"

ULevelLoadingManagerImpl::ULevelLoadingManagerImpl()
{
	TimeToWaitBeforeFadingInCamera = 2.f;
	CameraFadeLength = 2.f;
}

void ULevelLoadingManagerImpl::InitLoadingManager(TSoftObjectPtr<UWorld> PostProcessLevel)
{
	PersistentWorld = GetWorld();
	UWorldUtils::PersistentWorld = PersistentWorld;
	BasePostProcessWorld = PostProcessLevel;
	Internal_LoadBasePostProcessLevel();
}

void ULevelLoadingManagerImpl::LoadLevelTypeWithContext(const FLevelLoadContext& LevelLoadContext)
{	
	float ChangeLevelDelay = 0.f;
	
	if (LevelLoadContext.bShouldCameraFade)
	{
		ChangeLevelDelay = CameraFadeLength;
		Internal_PlayCameraFade(false);
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitBeforeLevelChange, [=]
	{
		Internal_LoadNewLevelWithContext(LevelLoadContext);
	}, ChangeLevelDelay + .1f, false);
}

void ULevelLoadingManagerImpl::Internal_ApplyLoadingStateToPlayerCharacter(const bool bAddingTags) const
{
	if (bAddingTags)
	{
		UGameplayTagComponent::AddTagToActor(UCoreUtils::GetCommonPlayerCharacter(this), TAG_STATE_LOADING);
		UGameplayTagComponent::AddTagToActor(UCoreUtils::GetCommonPlayerCharacter(this), TAG_STATE_STUNNED);
	}
	else
	{
		UGameplayTagComponent::RemoveTagFromActor(UCoreUtils::GetCommonPlayerCharacter(this), TAG_STATE_LOADING);
		UGameplayTagComponent::RemoveTagFromActor(UCoreUtils::GetCommonPlayerCharacter(this), TAG_STATE_STUNNED);
	}
}

void ULevelLoadingManagerImpl::Internal_PlayCameraFade(const bool bFadingIn) const
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (!CameraManager)
	{
		return;
	}

	if (bFadingIn)
	{
		CameraManager->StartCameraFade(1.f, 0.f, CameraFadeLength, FLinearColor::Black, false, true);
	}
	else
	{
		CameraManager->StartCameraFade(0.f, 1.f, CameraFadeLength, FLinearColor::Black, false, true);
	}
}

void ULevelLoadingManagerImpl::Internal_LoadNewLevelWithContext(const FLevelLoadContext& LevelLoadContext)
{
	PreviousLevelContext = CurrentLevelContext;
	CurrentLevelContext = LevelLoadContext;
	Internal_UnloadPreviousLevelWithContext();
}

void ULevelLoadingManagerImpl::Internal_LoadBasePostProcessLevel()
{
	const FLatentActionInfo LatentActionInfo = FLatentActionInfo(0, 0, TEXT("Internal_BasePostProcessLoaded"), this);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, BasePostProcessWorld, true, true, LatentActionInfo);
}

void ULevelLoadingManagerImpl::Internal_BasePostProcessLoaded()
{

}

void ULevelLoadingManagerImpl::Internal_UnloadPreviousLevelWithContext()
{
	const FLatentActionInfo LatentActionInfo = FLatentActionInfo(0, 0, TEXT("Internal_UnloadPreviousLightingLevel"), this);
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, PreviousLevelContext.BaseLevelToLoad, LatentActionInfo, true);
}

void ULevelLoadingManagerImpl::Internal_UnloadPreviousLightingLevel()
{
	const FLatentActionInfo LatentActionInfo = FLatentActionInfo(0, 0, TEXT("Internal_PostLevelUnload"), this);
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, PreviousLevelContext.LightingLevelToLoad, LatentActionInfo, true);
}

void ULevelLoadingManagerImpl::Internal_PostLevelUnload()
{
	Internal_LoadBaseLevel();
}

void ULevelLoadingManagerImpl::Internal_LoadBaseLevel()
{
	const FLatentActionInfo LatentActionInfo = FLatentActionInfo(0, 0, TEXT("Internal_LoadLightingLevel"), this);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, CurrentLevelContext.BaseLevelToLoad, true, true, LatentActionInfo);
}

void ULevelLoadingManagerImpl::Internal_LoadLightingLevel()
{
	const FLatentActionInfo LatentActionInfo = FLatentActionInfo(0, 0, TEXT("Internal_PostLevelLoad"), this);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, CurrentLevelContext.LightingLevelToLoad, true, true, LatentActionInfo);
}

void ULevelLoadingManagerImpl::Internal_PostLevelLoad()
{
	if (CurrentLevelContext.bShouldCameraFade)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitBeforeLevelChange, [=]
		{
			Internal_PlayCameraFade(true);
		}, TimeToWaitBeforeFadingInCamera, false);
	}
	CurrentStreamedWorld = CurrentLevelContext.BaseLevelToLoad->GetWorld();
	NewLevelLoadedEvent.Broadcast(FNewLevelLoadedEventPayload(CurrentStreamedWorld));
}
