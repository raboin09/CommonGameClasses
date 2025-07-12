// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonInteractTypes.h"
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
	TArray<TWeakObjectPtr<AActor>> Actors;
};

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UActorTrackingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UActorTrackingSubsystem* GetSubsystemFromWorld(const TWeakObjectPtr<UWorld> World);
	static UActorTrackingSubsystem* GetSubsystemFromActor(const TWeakObjectPtr<AActor> Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|ActorTracking")
	TArray<AActor*> GetAllActorsOfClass_TrackedOnly(TSubclassOf<AActor> ActorClass);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|ActorTracking")
	TArray<AActor*> GetAllActorsOfAffiliation_TrackedOnly(EAffiliation Affiliation);
	
	void TryAddActorToTrackedArrays(AActor* InActor);
	
	UPROPERTY()
	TMap<EAffiliation, FWorldActorTrackerEntry> ActorsOfAffiliation;
	UPROPERTY()
	TMap<UClass*, FWorldActorTrackerEntry> ActorsOfClass;
};
