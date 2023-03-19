// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonTypes.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class COMMONGAMECLASSES_API IInteractable
{
	GENERATED_BODY()

public:
	virtual EAffiliation GetAffiliation() const PURE_VIRTUAL(IInteractable::GetAffiliation, return EAffiliation::Neutral;)
	virtual void SwitchOutlineOnMesh(bool bShouldOutline) PURE_VIRTUAL(IInteractable::SwitchOutlineOnMesh, );
	virtual void InteractWithActor(AActor* InstigatingActor) PURE_VIRTUAL(IInteractable::InteractWithActor, );
	virtual FString GetInteractionText() const PURE_VIRTUAL(IInteractable::GetInteractionText, return "";);
};
