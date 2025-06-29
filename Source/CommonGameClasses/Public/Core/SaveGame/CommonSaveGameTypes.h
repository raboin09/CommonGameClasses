﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/Object.h"
#include "CommonSaveGameTypes.generated.h"


class ISavableActor;

USTRUCT()
struct FCommonActorSaveData
{
	GENERATED_BODY()

public:
	FCommonActorSaveData()
	{
		// Shouldn't do this, should only use the other constructors
	}
	
	explicit FCommonActorSaveData(AActor* Actor);
	explicit FCommonActorSaveData(const TScriptInterface<ISavableActor>& Actor);

	void AssignActorSaveData(AActor* Actor);
	
	static void SaveSerializedData(AActor* Actor, FMemoryWriter& MemoryWriter);
	static void SaveSerializedData(const TScriptInterface<ISavableActor>& Actor, FMemoryWriter& MemoryWriter);
	
	static void LoadSerializedData(AActor* Actor, FMemoryReader& MemoryReader);
	static void LoadSerializedData(const TScriptInterface<ISavableActor>& Actor, FMemoryReader& MemoryReader);
	
	UPROPERTY()
	FName ActorUniqueId;
	UPROPERTY()
	FTransform SavedTransform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
	TArray<uint8> ByteData;

private:
	static void GetSerializedData(AActor* Actor, FObjectAndNameAsStringProxyArchive& Archiver);
};