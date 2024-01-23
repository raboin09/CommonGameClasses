// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/CommonAssetTypes.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Types/CommonEventDeclarations.h"
#include "ActorAssetManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UActorAssetManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActorAssetManagerComponent();
	
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadNiagaraSystemObject(TSoftObjectPtr<UNiagaraSystem> UnloadedAsset, const bool bIsHighPriority, FLoadedNiagaraSystemObjectEvent LoadedObjectEvent);
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadParticleSystemObject(TSoftObjectPtr<UParticleSystem> UnloadedAsset, const bool bIsHighPriority, FLoadedCascadeSystemObjectEvent LoadedObjectEvent);
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadSoundObject(TSoftObjectPtr<USoundBase> UnloadedAsset, const bool bIsHighPriority, FLoadedSoundObjectEvent LoadedObjectEvent);
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadTextureObject(TSoftObjectPtr<UTexture> UnloadedAsset, const bool bIsHighPriority, FLoadedTextureEvent LoadedObjectEvent);
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadMaterialObject(TSoftObjectPtr<UMaterialInterface> UnloadedAsset, const bool bIsHighPriority, FLoadedMaterialEvent LoadedObjectEvent);
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadStaticMeshObject(TSoftObjectPtr<UStaticMesh> UnloadedAsset, const bool bIsHighPriority, FLoadedStaticMeshEvent LoadedObjectEvent);
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadSkeletalMeshObject(TSoftObjectPtr<USkeletalMesh> UnloadedAsset, const bool bIsHighPriority, FLoadedSkeletalMeshEvent LoadedObjectEvent);
	UFUNCTION(BlueprintCallable, Category="COMMON|AssetManagement")
	void K2_Async_LoadClassObject(TSoftClassPtr<UClass> UnloadedAsset, const bool bIsHighPriority, FLoadedClassEvent LoadedObjectEvent);

private:
	template<typename AssetType, typename DelegateType>
	void Async_LoadSoftObjects(TArray<TAsyncObjectLoadRequest<AssetType, DelegateType>> AssetLoadRequests);
	template<typename AssetType, typename DelegateType>
	void Async_LoadSoftObject(TSoftObjectPtr<AssetType> UnloadedAsset, const bool bIsHighPriority, DelegateType LoadedObjectEvent);
	template<typename AssetType, typename DelegateType>
	void Async_LoadSoftClass(TSoftClassPtr<AssetType> UnloadedAsset, const bool bIsHighPriority, DelegateType LoadedObjectEvent);

	void Async_LoadSoftReference(const FSoftObjectPath& UnloadedAssetPath, const bool bIsHighPriority, const FStreamableDelegate& StreamableDelegate)
	{
		const TAsyncLoadPriority& LoadPriority = bIsHighPriority ? FStreamableManager::AsyncLoadHighPriority : FStreamableManager::DefaultAsyncLoadPriority;
		const TSharedPtr<FStreamableHandle> Handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(UnloadedAssetPath, StreamableDelegate, LoadPriority, false, false, TEXT("ActorAssetManager"));
		RequestedStreamHandles.Add(Handle);
	}
	
	TSet<TSharedPtr<FStreamableHandle>> RequestedStreamHandles;
};

template<typename AssetType, typename DelegateType>
void UActorAssetManagerComponent::Async_LoadSoftObjects(TArray<TAsyncObjectLoadRequest<AssetType, DelegateType>> AssetLoadRequests)
{
	for(TAsyncObjectLoadRequest<AssetType, DelegateType>& AssetLoadRequest : AssetLoadRequests)
	{
		Async_LoadSoftObject(AssetLoadRequest);
	}
}

template<typename AssetType, typename DelegateType>
void UActorAssetManagerComponent::Async_LoadSoftObject(TSoftObjectPtr<AssetType> UnloadedAsset, const bool bIsHighPriority, DelegateType LoadedObjectEvent)
{
	FStreamableDelegate StreamableDelegate = FStreamableDelegate();
	StreamableDelegate.BindUFunction(LoadedObjectEvent.GetUObject(), LoadedObjectEvent.GetFunctionName(), UnloadedAsset);
	Async_LoadSoftReference(UnloadedAsset.ToSoftObjectPath(), bIsHighPriority, StreamableDelegate);
}

template<typename AssetType, typename DelegateType>
void UActorAssetManagerComponent::Async_LoadSoftClass(TSoftClassPtr<AssetType> UnloadedAsset, const bool bIsHighPriority, DelegateType LoadedObjectEvent)
{
	FStreamableDelegate StreamableDelegate = FStreamableDelegate();
	StreamableDelegate.BindUFunction(LoadedObjectEvent.GetUObject(), LoadedObjectEvent.GetFunctionName(), UnloadedAsset);
	Async_LoadSoftReference(UnloadedAsset.ToSoftObjectPath(), bIsHighPriority, StreamableDelegate);
}