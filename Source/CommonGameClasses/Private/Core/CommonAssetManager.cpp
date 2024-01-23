// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonAssetManager.h"

UCommonAssetManager& UCommonAssetManager::Get()
{
	check(GEngine);

	if (UCommonAssetManager* Singleton = Cast<UCommonAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	// Fatal error above prevents this from being called.
	return *NewObject<UCommonAssetManager>();
}

UObject* UCommonAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (IsInitialized())
		{
			return GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UCommonAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}
