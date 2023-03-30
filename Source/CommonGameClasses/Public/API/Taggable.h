// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/EventDeclarations.h"
#include "UObject/Interface.h"
#include "Taggable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UTaggable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API ITaggable
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload) PURE_VIRTUAL(ITaggable::HandleTagAdded, )
	UFUNCTION()
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload) PURE_VIRTUAL(ITaggable::HandleTagRemoved, )
};
