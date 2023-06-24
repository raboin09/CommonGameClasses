// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/CommonTypes.h"
#include "Types/CommonEventDeclarations.h"
#include "CommonWorldUtils.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonWorldUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//////////////////////////
	// Static Variables
	//////////////////////////
	static TArray<AActor*> QuestRelevantActors;
	static TArray<AActor*> AlliedActors;
	static TArray<AActor*> EnemyActors;
	
	static UWorld* PersistentWorld;
	static UWorld* CurrentStreamedWorld;

	UFUNCTION()
	static void HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload); 
	
	static void TryAddActorToTeamArray(AActor* InActor, EAffiliation AbsoluteAffiliation);
	static void TryAddActorToQuestableArray(AActor* InActor);
	static void TryRemoveActorFromQuestableArray(AActor* InActor);

	////////////////////////////////////////////////////////////////////
	/// Get all actors of world
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SpawnUtils")
	static TArray<AActor*> GetAllActorsOfClassInPersistentWorld(TSubclassOf<AActor> ActorClass);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SpawnUtils")
	static TArray<AActor*> GetAllActorsOfClassInCurrentStreamedWorld(TSubclassOf<AActor> ActorClass);

	////////////////////////////////////////////////////////////////////
	/// K2 Spawn deferred
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category="COMMON|WorldUtils")
	static AActor* K2_SpawnActorToCurrentStreamedWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	UFUNCTION(BlueprintCallable, Category="COMMON|WorldUtils")
	static AActor* K2_SpawnActorToPersistentWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	UFUNCTION(BlueprintCallable)
	static void FinishSpawningActor_Deferred(AActor* InActor, const FTransform& ActorTransform);

	////////////////////////////////////////////////////////////////////
	/// K2 Spawn instantly
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	static AActor* K2_SpawnActorToCurrentStreamedWorld(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform);
	UFUNCTION(BlueprintCallable)
	static AActor* K2_SpawnActorToPersistentWorld(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform);

	////////////////////////////////////////////////////////////////////
	/// Native Spawn deferred
	////////////////////////////////////////////////////////////////////
	template<typename T>
	FORCEINLINE static T* SpawnActorToPersistentWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		return Internal_TemplatedSpawnActorFromClass<T>(PersistentWorld, ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
	}
	
	template<typename T>
	FORCEINLINE static T* SpawnActorToCurrentStreamedWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		return Internal_TemplatedSpawnActorFromClass<T>(CurrentStreamedWorld, ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
	}
	
private:
	template<typename T>
	FORCEINLINE static T* Internal_TemplatedSpawnActorFromClass(UWorld* World, TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		if(!World)
		{
			return nullptr;
		}
		return World->SpawnActorDeferred<T>(ClassToSpawn, FTransform(), Owner, Instigator, CollisionHandlingOverride);
	}

	static AActor* Internal_SpawnActorFromClass(UWorld* World, UClass* Class, const FTransform& SpawnTransform);
	static TArray<AActor*> Internal_GetAllActorsFromWorld(UWorld* World, TSubclassOf<AActor> ActorClass);
};