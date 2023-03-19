// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Interactable.h"
#include "Character/HealthComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Quest/QuestManagerComponent.h"
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
	static TScriptInterface<IInteractable> GetHoveredActorByPlayerController(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UHealthComponent* GetPlayerCharacterHealthComponent(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UHealthComponent* GetHealthComponentFromActor(UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Genestealer|CoreUtils")
	static UQuestManagerComponent* GetQuestManager(const UObject* ContextObject);
};
