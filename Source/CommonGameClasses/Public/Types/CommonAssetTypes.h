// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class FAssetFinishedLoadingCallback;

template<typename AssetType, typename DelegateType>
struct TAsyncObjectLoadRequest
{
	TSoftObjectPtr<AssetType> AssetToLoad;
	DelegateType Callback;
	bool bHighPriority = false;
};

template<typename AssetType,typename DelegateType>
struct TAsyncClassLoadRequest
{
	TSoftClassPtr<AssetType> AssetToLoad;
	DelegateType Callback;
	bool bHighPriority = false;
};
