// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreUtils.generated.h"

class ACommonPlayerCharacter;
class ACommonPlayerController;

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCoreUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool IsObjectPlayerControlled(const UObject* Object);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static ACommonPlayerCharacter* GetCommonPlayerCharacter(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static ACommonPlayerController* GetCommonPlayerController(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static TScriptInterface<IInteractable> GetTargetedActorByPlayerController(const UObject* ContextObject);
};
