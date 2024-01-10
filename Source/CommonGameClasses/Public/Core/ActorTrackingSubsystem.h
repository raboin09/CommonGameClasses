// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonTypes.h"
#include "UObject/Object.h"
#include "ActorTrackingSubsystem.generated.h"

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
UCLASS()
class COMMONGAMECLASSES_API UActorTrackingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UActorTrackingSubsystem* GetSubsystemFromWorld(const UWorld* World);
	static UActorTrackingSubsystem* GetSubsystemFromActor(const AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|ActorTracking")
	TArray<AActor*> GetAllActorsOfClass_TrackedOnly(TSubclassOf<AActor> ActorClass);
	
	void EmptyQuestRelevantArray();
	void TryAddActorToTrackedArrays(AActor* InActor);
	void TryRemoveActorFromQuestableArray(AActor* InActor);
	
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
