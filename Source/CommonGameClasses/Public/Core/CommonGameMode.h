// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonTypes.h"
#include "GameFramework/GameModeBase.h"
#include "CommonGameMode.generated.h"

USTRUCT()
struct FWorldActorTrackerEntry
{
	GENERATED_BODY()

	FWorldActorTrackerEntry() {}
	FWorldActorTrackerEntry(const TArray<AActor*>& InActor) : Actors(InActor) {};
	FWorldActorTrackerEntry(AActor* InActor)
	{
		Actors.Empty();
		Actors.Add(InActor);
	};
	
	UPROPERTY()
	TArray<AActor*> Actors;
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	static void EmptyQuestRelevantArray(const UObject* ContextObject);
	static void TryAddActorToTrackedArrays(AActor* InActor);
	static void TryRemoveActorFromQuestableArray(AActor* InActor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameMode")
	static TArray<AActor*> GetAllActorsOfClass_TrackedOnly(const UObject* ContextObject, TSubclassOf<AActor> ActorClass);
	
	UPROPERTY()
	TArray<AActor*> QuestRelevantActors;
	UPROPERTY()
	TArray<AActor*> AlliedActors;
	UPROPERTY()
	TArray<AActor*> EnemyActors;
	UPROPERTY()
	TMap<EAffiliation, FWorldActorTrackerEntry> ActorsOfAffiliation;
	UPROPERTY()
	TMap<UClass*, FWorldActorTrackerEntry> ActorsOfClass;

};
