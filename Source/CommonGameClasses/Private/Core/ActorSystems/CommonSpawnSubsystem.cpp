﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ActorSystems/CommonSpawnSubsystem.h"

AActor* UCommonSpawnSubsystem::BPI_SpawnActorToCurrentWorld_Deferred(const UObject* ContextObject, TSubclassOf<AActor> ClassToSpawn, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride)
{
	return SpawnActorToCurrentWorld_Deferred<AActor>(ContextObject, ClassToSpawn, Owner, Instigator, CollisionHandlingOverride);
}

void UCommonSpawnSubsystem::FinishSpawningActor_Deferred(AActor* InActor, const FTransform& ActorTransform)
{
	if(!InActor)
	{
		return;
	}
	InActor->FinishSpawning(ActorTransform);
}

AActor* UCommonSpawnSubsystem::BPI_SpawnActorToCurrentWorld(const UObject* ContextObject, TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride)
{
	const UWorld* CurrWorld = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::ReturnNull);
	check(CurrWorld)
	if(UCommonSpawnSubsystem* CommonSpawnSubsystem = CurrWorld->GetSubsystem<UCommonSpawnSubsystem>())
	{
		return CommonSpawnSubsystem->Internal_TemplatedSpawnActorFromClass<AActor>(ClassToSpawn, SpawnTransform, Owner, Instigator, CollisionHandlingOverride);
	}
	return nullptr;
}