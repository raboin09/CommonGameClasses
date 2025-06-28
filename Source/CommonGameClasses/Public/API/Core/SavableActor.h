// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SaveGame/CommonSaveGameTypes.h"
#include "UObject/Interface.h"
#include "SavableActor.generated.h"



UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USavableActor : public UInterface{ GENERATED_BODY() };
class COMMONGAMECLASSES_API ISavableActor
{
	GENERATED_BODY()

public:
	virtual void PreActorSaved() {}
	virtual void PostActorSaved(const FCommonActorSaveData& InActorSaveData) {}
	virtual void PostActorLoadedFromSave(const FCommonActorSaveData& InActorSaveData) PURE_VIRTUAL(ISavable::PostActorLoadedFromSave, )
};
