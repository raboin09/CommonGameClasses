// Fill out your copyright notice in the Description page of Project CUSTOM.

#pragma once

#include "CoreMinimal.h"
#include "Character/CommonCharacter.h"
#include "CommonPlayerCharacter.generated.h"

class UCameraComponent;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM", "CUSTOM|Defaults"), PrioritizeCategories = "CUSTOM")
class COMMONGAMECLASSES_API ACommonPlayerCharacter : public ACommonCharacter
{
	GENERATED_BODY()
};
