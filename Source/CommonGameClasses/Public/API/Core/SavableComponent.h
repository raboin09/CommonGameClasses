// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableComponent.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USavableComponent : public UInterface{ GENERATED_BODY() };
class COMMONGAMECLASSES_API ISavableComponent
{
	GENERATED_BODY()
	
public:

	virtual void PreComponentSaved() {}
	virtual void PostComponentSaved() {}
	virtual void PostComponentLoadedFromSave() {}
};
