// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/CommonWorldUtils.h"
#include "Ability/CommonAbility.h"

UWorld* UCommonWorldUtils::CurrentWorld = nullptr;

AActor* UCommonWorldUtils::K2_SpawnActorToCurrentWorld_Deferred(TSubclassOf<AActor> ClassToSpawn, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride)
{
	return SpawnActorToCurrentWorld_Deferred<AActor>(ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
}

void UCommonWorldUtils::HandleNewLevelLoadEvent(const FNewLevelLoadedEventPayload& NewLevelLoadedPayload)
{
	CurrentWorld = NewLevelLoadedPayload.NewStreamedWorld;
}

void UCommonWorldUtils::FinishSpawningActor_Deferred(AActor* InActor, const FTransform& ActorTransform)
{
	if(!InActor)
	{
		return;
	}
	InActor->FinishSpawning(ActorTransform);
}

AActor* UCommonWorldUtils::K2_SpawnActorToCurrentWorld(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride)
{
	return Internal_TemplatedSpawnActorFromClass<AActor>(ClassToSpawn, SpawnTransform, Owner, Instigator, CollisionHandlingOverride);
}