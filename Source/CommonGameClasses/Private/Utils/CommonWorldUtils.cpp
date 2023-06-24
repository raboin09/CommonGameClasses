// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/CommonWorldUtils.h"
#include "EngineUtils.h"
#include "API/Questable.h"

TArray<AActor*> UCommonWorldUtils::QuestRelevantActors = {};
TArray<AActor*> UCommonWorldUtils::AlliedActors = {};
TArray<AActor*> UCommonWorldUtils::EnemyActors = {};
UWorld* UCommonWorldUtils::PersistentWorld = nullptr;
UWorld* UCommonWorldUtils::CurrentStreamedWorld = nullptr;

void UCommonWorldUtils::TryAddActorToTeamArray(AActor* InActor, EAffiliation AbsoluteAffiliation)
{
	switch (AbsoluteAffiliation) {
		case EAffiliation::Allies:
			AlliedActors.AddUnique(InActor);
			break;
		case EAffiliation::Enemies:
			EnemyActors.AddUnique(InActor);
			break;
		case EAffiliation::Neutral:
		case EAffiliation::Destructible:
		default: break;
	}
}

void UCommonWorldUtils::TryAddActorToQuestableArray(AActor* InActor)
{
	if(!InActor || !InActor->GetClass() || !InActor->GetClass()->ImplementsInterface(UQuestable::StaticClass()))
	{
		return;
	}
	QuestRelevantActors.AddUnique(InActor);
}

void UCommonWorldUtils::TryRemoveActorFromQuestableArray(AActor* InActor)
{
	if(!InActor || !InActor->GetClass() || !InActor->GetClass()->ImplementsInterface(UQuestable::StaticClass()))
	{
		return;
	}
	QuestRelevantActors.Remove(InActor);
}

TArray<AActor*> UCommonWorldUtils::GetAllActorsOfClassInPersistentWorld(TSubclassOf<AActor> ActorClass)
{
	return Internal_GetAllActorsFromWorld(PersistentWorld, ActorClass);
}

TArray<AActor*> UCommonWorldUtils::GetAllActorsOfClassInCurrentStreamedWorld(TSubclassOf<AActor> ActorClass)
{
	return Internal_GetAllActorsFromWorld(CurrentStreamedWorld, ActorClass);
}

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

TArray<AActor*> UCommonWorldUtils::Internal_GetAllActorsFromWorld(UWorld* World, TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> OutActors;
	if(!World)
	{
		return OutActors;
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
