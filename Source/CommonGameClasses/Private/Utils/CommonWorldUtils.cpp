// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/CommonWorldUtils.h"
#include "EngineUtils.h"
#include "Ability/CommonAbility.h"
#include "Kismet/KismetMathLibrary.h"

UWorld* UCommonWorldUtils::PersistentWorld = nullptr;
UWorld* UCommonWorldUtils::CurrentStreamedWorld = nullptr;

AActor* UCommonWorldUtils::K2_SpawnActorToCurrentStreamedWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride)
{
	return SpawnActorToCurrentStreamedWorld_Deferred<AActor>(ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
}

AActor* UCommonWorldUtils::K2_SpawnActorToPersistentWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride)
{
	return SpawnActorToPersistentWorld_Deferred<AActor>(ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
}

void UCommonWorldUtils::HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload)
{
	CurrentStreamedWorld = NewLevelLoadedPayload.NewStreamedWorld;
}

void UCommonWorldUtils::FinishSpawningActor_Deferred(AActor* InActor, const FTransform& ActorTransform)
{
	if(!InActor)
	{
		return;
	}
	InActor->FinishSpawning(ActorTransform);
}

AActor* UCommonWorldUtils::K2_SpawnActorToCurrentStreamedWorld(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform)
{
	return Internal_SpawnActorFromClass(CurrentStreamedWorld, ClassToSpawn, SpawnTransform);
}

AActor* UCommonWorldUtils::K2_SpawnActorToPersistentWorld(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform)
{
	return Internal_SpawnActorFromClass(PersistentWorld, ClassToSpawn, SpawnTransform);
}

AActor* UCommonWorldUtils::Internal_SpawnActorFromClass(UWorld* World, UClass* Class, const FTransform& SpawnTransform)
{
	if(!World)
	{
		return nullptr;
	}
	return World->SpawnActor<AActor>(Class, SpawnTransform);
}

TArray<AActor*> UCommonWorldUtils::GetAllActorsOfClassOfWorld(TSubclassOf<AActor> ActorClass, bool bStreamedWorld)
{
	TArray<AActor*> OutActors;
	const UWorld* World = bStreamedWorld ? CurrentStreamedWorld : PersistentWorld;
	if(!World)
	{
		return {};
	}
	
	if (ActorClass)
	{
		for(TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;
			OutActors.Add(Actor);
		}
	}
	return OutActors;
}