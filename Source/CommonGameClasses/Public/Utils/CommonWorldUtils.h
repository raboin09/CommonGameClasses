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
	static UWorld* CurrentWorld;
	
	UFUNCTION()
	static void HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload); 

	////////////////////////////////////////////////////////////////////
	/// K2 Spawn deferred
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category="COMMON|WorldUtils")
	static AActor* K2_SpawnActorToCurrentWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	UFUNCTION(BlueprintCallable)
	static void FinishSpawningActor_Deferred(AActor* InActor, const FTransform& ActorTransform);

	////////////////////////////////////////////////////////////////////
	/// K2 Spawn instantly
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	static AActor* K2_SpawnActorToCurrentWorld(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	////////////////////////////////////////////////////////////////////
	/// Native Spawn deferred
	////////////////////////////////////////////////////////////////////
	template<typename T>
	FORCEINLINE static T* SpawnActorToCurrentWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		return Internal_TemplatedSpawnActorFromClass_Deferred<T>(ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
	}
	
private:
	
	template<typename T>
	FORCEINLINE static T* Internal_TemplatedSpawnActorFromClass_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		if(!CurrentWorld && (!Owner || !Owner->GetWorld()))
		{
			return nullptr;
		}
		UWorld* WorldToSpawnTo = CurrentWorld ? CurrentWorld : Owner->GetWorld();
		return WorldToSpawnTo->SpawnActorDeferred<T>(ClassToSpawn, FTransform(), Owner, Instigator, CollisionHandlingOverride);
	}

	template<typename T>
	static T* Internal_TemplatedSpawnActorFromClass(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		if(!CurrentWorld && (!Owner || !Owner->GetWorld()))
		{
			return nullptr;
		}
		UWorld* WorldToSpawnTo = CurrentWorld ? CurrentWorld : Owner->GetWorld();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;
		SpawnParameters.Instigator = Instigator;
		SpawnParameters.SpawnCollisionHandlingOverride = CollisionHandlingOverride;
		return WorldToSpawnTo->SpawnActor<T>(ClassToSpawn, SpawnTransform, SpawnParameters);
	}
};