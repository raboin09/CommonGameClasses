// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonTypes.h"
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
	static EAffiliation GetAffiliationRelatedToPlayerCharacter(AActor* ContextActor);
	static EAffiliation GetAffiliationOfActor(const AActor* InActor);
	static bool AreActorsAllies(const AActor* FirstActor, const AActor* SecondActor);
	static bool AreActorsEnemies(const AActor* FirstActor, const AActor* SecondActor);
	static bool IsActorDestructible(const AActor* FirstActor);
	static bool IsActorNeutral(const AActor* FirstActor);
	static int32 GetOutlineInt(const AActor* InActor);
};
