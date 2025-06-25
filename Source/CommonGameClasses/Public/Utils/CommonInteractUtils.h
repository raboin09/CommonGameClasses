// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Types\CommonInteractTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonInteractUtils.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonInteractUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Interact")
	static EAffiliation GetAffiliationRelatedToPlayerCharacter(AActor* ContextActor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Interact")
	static EAffiliation GetAffiliationOfActor(const AActor* InActor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Interact")
	static bool AreActorsAllies(const AActor* FirstActor, const AActor* SecondActor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Interact")
	static bool AreActorsEnemies(const AActor* FirstActor, const AActor* SecondActor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Interact")
	static bool IsActorDestructible(const AActor* FirstActor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Interact")
	static bool IsActorNeutral(const AActor* FirstActor);
	static int32 GetOutlineInt(const AActor* InActor);
};
