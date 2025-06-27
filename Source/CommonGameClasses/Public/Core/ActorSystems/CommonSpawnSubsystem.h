// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "CommonSpawnSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	
	////////////////////////////////////////////////////////////////////
	/// K2 Spawn deferred
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category="COMMON|WorldUtils")
	static AActor* K2_SpawnActorToCurrentWorld_Deferred(const UObject* ContextObject, TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	UFUNCTION(BlueprintCallable)
	static void FinishSpawningActor_Deferred(AActor* InActor, const FTransform& ActorTransform);

	////////////////////////////////////////////////////////////////////
	/// K2 Spawn instantly
	////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	static AActor* K2_SpawnActorToCurrentWorld(const UObject* ContextObject, TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	////////////////////////////////////////////////////////////////////
	/// Native Spawn deferred
	////////////////////////////////////////////////////////////////////
	template<typename T>
	FORCEINLINE static T* SpawnActorToCurrentWorld_Deferred(const UObject* ContextObject, TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		const UWorld* CurrWorld = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::ReturnNull);
		check(CurrWorld)
		if(UCommonSpawnSubsystem* CommonSpawnSubsystem = CurrWorld->GetSubsystem<UCommonSpawnSubsystem>())
		{
			return CommonSpawnSubsystem->Internal_TemplatedSpawnActorFromClass_Deferred<T>(ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);	
		}
		return nullptr;
	}
	
private:
	
	template<typename T>
	FORCEINLINE T* Internal_TemplatedSpawnActorFromClass_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		if(!GetWorld() && (!Owner || !Owner->GetWorld()))
		{
			return nullptr;
		}
		return GetWorld()->SpawnActorDeferred<T>(ClassToSpawn, FTransform(), Owner, Instigator, CollisionHandlingOverride);
	}

	template<typename T>
	T* Internal_TemplatedSpawnActorFromClass(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform, AActor* Owner = nullptr, APawn* Instigator = nullptr, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		if(!GetWorld() && (!Owner || !Owner->GetWorld()))
		{
			return nullptr;
		}
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;
		SpawnParameters.Instigator = Instigator;
		SpawnParameters.SpawnCollisionHandlingOverride = CollisionHandlingOverride;
		return GetWorld()->SpawnActor<T>(ClassToSpawn, SpawnTransform, SpawnParameters);
	}
};