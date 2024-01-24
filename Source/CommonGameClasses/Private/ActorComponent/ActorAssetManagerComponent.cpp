// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/ActorAssetManagerComponent.h"

#include "NiagaraSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonEventDeclarations.h"


UActorAssetManagerComponent::UActorAssetManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorAssetManagerComponent::K2_Async_LoadAnimMontageObject(TSoftObjectPtr<UAnimMontage> UnloadedAsset, const bool bIsHighPriority, FLoadedAnimMontageEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<UAnimMontage, FLoadedAnimMontageEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
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

void UActorAssetManagerComponent::K2_Async_LoadBehaviorTreeObject(TSoftObjectPtr<UBehaviorTree> UnloadedAsset, const bool bIsHighPriority, FLoadedBehaviorTreeEvent LoadedObjectEvent)
{
	Async_LoadSoftObject<UBehaviorTree, FLoadedBehaviorTreeEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadAbilityClass(TSoftClassPtr<ACommonAbility> UnloadedAsset, const bool bIsHighPriority, FLoadedAbilityEvent LoadedObjectEvent)
{
	Async_LoadSoftClass<ACommonAbility, FLoadedAbilityEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadActorClassObject(TSoftClassPtr<ACommonActor> UnloadedAsset, const bool bIsHighPriority, FLoadedActorEvent LoadedObjectEvent)
{
	Async_LoadSoftClass<ACommonActor, FLoadedActorEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

void UActorAssetManagerComponent::K2_Async_LoadCharacterClassObject(TSoftClassPtr<ACommonCharacter> UnloadedAsset, const bool bIsHighPriority, FLoadedCharacterEvent LoadedObjectEvent)
{
	Async_LoadSoftClass<ACommonCharacter, FLoadedCharacterEvent>(UnloadedAsset, bIsHighPriority, LoadedObjectEvent);
}

