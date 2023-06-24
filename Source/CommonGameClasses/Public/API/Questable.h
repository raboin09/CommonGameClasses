// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "UObject/Interface.h"
#include "Questable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UQuestable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IQuestable
{
	GENERATED_BODY()

public:
	virtual FQuestObjectiveEvent& OnQuestObjectiveEvent() = 0;
};
