// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/ActorAssetManagerComponent.h"

#include "NiagaraSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonEventDeclarations.h"


UActorAssetManagerComponent::UActorAssetManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorAssetManagerComponent::K2_Async_LoadNiagaraSystemObject(TSoftObjectPtr<UNiagaraSystem> UnloadedAsset, const bool bIsHighPriority, FLoadedNiagaraSystemObjectEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<UNiagaraSystem, FLoadedNiagaraSystemObjectEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadParticleSystemObject(TSoftObjectPtr<UParticleSystem> UnloadedAsset, const bool bIsHighPriority, FLoadedCascadeSystemObjectEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<UParticleSystem, FLoadedCascadeSystemObjectEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadSoundObject(TSoftObjectPtr<USoundBase> UnloadedAsset, const bool bIsHighPriority, FLoadedSoundObjectEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<USoundBase, FLoadedSoundObjectEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadTextureObject(TSoftObjectPtr<UTexture> UnloadedAsset, const bool bIsHighPriority, FLoadedTextureEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<UTexture, FLoadedTextureEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadMaterialObject(TSoftObjectPtr<UMaterialInterface> UnloadedAsset, const bool bIsHighPriority, FLoadedMaterialEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<UMaterialInterface, FLoadedMaterialEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadStaticMeshObject(TSoftObjectPtr<UStaticMesh> UnloadedAsset, const bool bIsHighPriority, FLoadedStaticMeshEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<UStaticMesh, FLoadedStaticMeshEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadSkeletalMeshObject(TSoftObjectPtr<USkeletalMesh> UnloadedAsset, const bool bIsHighPriority, FLoadedSkeletalMeshEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<USkeletalMesh, FLoadedSkeletalMeshEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadClassObject(TSoftClassPtr<UClass> UnloadedAsset, const bool bIsHighPriority, FLoadedClassEvent LoadedObjectEvent)
{
	Async_LoadSoftClass<UClass, FLoadedClassEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}
