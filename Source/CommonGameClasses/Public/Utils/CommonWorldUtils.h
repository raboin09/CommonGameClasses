// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	static UWorld* PersistentWorld;
	static UWorld* CurrentStreamedWorld;
	
	UFUNCTION()
	static void HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload); 

	////////////////////////////////////////////////////////////////////
	/// Get all actors of world
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SpawnUtils")
	static TArray<AActor*> GetAllActorsOfClassOfWorld(TSubclassOf<AActor> ActorClass, bool bStreamedWorld = false);

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
		return Internal_TemplatedSpawnActorFromClass<T>(PersistentWorld ? PersistentWorld : Owner->GetWorld(), ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
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
};