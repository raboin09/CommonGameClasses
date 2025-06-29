// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SaveGame/CommonSaveGameTypes.h"
#include "API/Core/SavableActor.h"
#include "API/Core/SavableComponent.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

class ISavableActor;

FCommonActorSaveData::FCommonActorSaveData(AActor* Actor)
{
	AssignActorSaveData(Actor);
}

FCommonActorSaveData::FCommonActorSaveData(const TScriptInterface<ISavableActor>& Actor)
{
	AssignActorSaveData(Cast<AActor>(Actor.GetObject()));
}

void FCommonActorSaveData::AssignActorSaveData(AActor* Actor)
{
	if(!IsValid(Actor))
	{
		return;;
	}
	ISavableActor* SavableActor = Cast<ISavableActor>(Actor);
	if(!SavableActor)
	{
		return;
	}
	ActorUniqueId = Actor->GetFName();
	SavedTransform = Actor->GetTransform();
}

void FCommonActorSaveData::SaveSerializedData(AActor* Actor, FMemoryWriter& MemoryWriter)
{
	FObjectAndNameAsStringProxyArchive Archiver = FObjectAndNameAsStringProxyArchive(MemoryWriter, true);
	GetSerializedData(Actor, Archiver);
}

void FCommonActorSaveData::LoadSerializedData(AActor* Actor, FMemoryReader& MemoryReader)
{
	FObjectAndNameAsStringProxyArchive Archiver = FObjectAndNameAsStringProxyArchive(MemoryReader, true);
	GetSerializedData(Actor, Archiver);
}

void FCommonActorSaveData::SaveSerializedData(const TScriptInterface<ISavableActor>& Actor, FMemoryWriter& MemoryWriter)
{
	SaveSerializedData(Cast<AActor>(Actor.GetObject()), MemoryWriter);
}

void FCommonActorSaveData::LoadSerializedData(const TScriptInterface<ISavableActor>& Actor, FMemoryReader& MemoryReader)
{
	LoadSerializedData(Cast<AActor>(Actor.GetObject()), MemoryReader);
}

void FCommonActorSaveData::GetSerializedData(AActor* Actor, FObjectAndNameAsStringProxyArchive& Archiver)
{
	Archiver.ArIsSaveGame = true;
	Archiver.ArNoDelta = true;
	Actor->Serialize(Archiver);
	TArray<UActorComponent*> ActorComponents = Actor->GetComponentsByInterface(USavableComponent::StaticClass());
	for(UActorComponent* ActorComponent : ActorComponents)
	{
		ISavableComponent* SavableComponent = Cast<ISavableComponent>(ActorComponent);
		ActorComponent->Serialize(Archiver);
	}
}
