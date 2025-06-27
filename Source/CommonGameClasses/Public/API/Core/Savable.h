// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Savable.generated.h"

USTRUCT()
struct FCommonActorSaveData
{
	GENERATED_BODY()

public:
	FCommonActorSaveData()
	{
		// Shouldn't do this, should only use the other constructors
	}
	
	explicit FCommonActorSaveData(const AActor* InActor)
	{
		if(!InActor)
		{
			return;
		}
		ActorUniqueId = InActor->GetFName();
		SavedTransform = InActor->GetTransform();
	}
	
	UPROPERTY()
	FName ActorUniqueId;
	UPROPERTY()
	FTransform SavedTransform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
	TArray<uint8> ByteData;
};


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USavable : public UInterface{ GENERATED_BODY() };
class COMMONGAMECLASSES_API ISavable
{
	GENERATED_BODY()

public:
	virtual FCommonActorSaveData CreateActorSaveData() const PURE_VIRTUAL(ISavable::CreateActorSaveData, return FCommonActorSaveData(); )
	virtual void ApplyActorSaveData(const FCommonActorSaveData& InActorSaveData) PURE_VIRTUAL(ISavable::ApplyActorSaveData, )
};
