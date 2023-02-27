// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.h"
#include "Interactable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractUtils.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UInteractUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static EAffiliation GetAffiliationRelatedToPlayerCharacter(AActor* ContextActor);
	static bool AreActorsAllies(TScriptInterface<IInteractable> FirstActor, AActor* SecondActor);
	static bool AreActorsAllies(AActor* FirstActor, AActor* SecondActor);
	UFUNCTION(BlueprintCallable, Category="COMMON|InteractUtils")
	static bool AreActorsEnemies(AActor* FirstActor, AActor* SecondActor);
	static bool AreActorsEnemies(TScriptInterface<IInteractable> FirstActor, AActor* SecondActor);
	static bool IsActorNeutral(AActor* FirstActor);
	static bool IsActorNeutral(TScriptInterface<IInteractable> FirstActor);
	static int32 GetOutlineInt(AActor* InActor);
};
