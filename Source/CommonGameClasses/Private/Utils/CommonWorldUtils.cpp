// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/CommonWorldUtils.h"
#include "EngineUtils.h"
#include "ActorComponent/InteractionComponent.h"
#include "API/Questable.h"
#include "Kismet/KismetMathLibrary.h"

TArray<AActor*> UCommonWorldUtils::QuestRelevantActors = {};

TMap<const EAffiliation, TArray<AActor*>> UCommonWorldUtils::ActorsOfAffiliation = {};
TMap<UClass*, TArray<AActor*>> UCommonWorldUtils::ActorsOfClass = {};

UWorld* UCommonWorldUtils::PersistentWorld = nullptr;
UWorld* UCommonWorldUtils::CurrentStreamedWorld = nullptr;

void UCommonWorldUtils::TryAddActorToTrackedArrays(AActor* InActor)
{
	if(!InActor)
	{
		return;
	}

	// Add to affiliation-relevant arrays 
	if(const UInteractionComponent* InteractionComponent = InActor->FindComponentByClass<UInteractionComponent>())
	{
		const EAffiliation Affiliation = InteractionComponent->Affiliation;
		if(!ActorsOfAffiliation.Contains(Affiliation))
		{
			ActorsOfAffiliation.Add(Affiliation, {InActor});
		} else
		{
			TArray<AActor*>& AffiliatedActors = *ActorsOfAffiliation.Find(Affiliation);
			AffiliatedActors.Add(InActor);		
		}
	}

	const TSubclassOf<AActor> ActorClass = InActor->GetClass();
	if(!ActorClass)
	{
		return;
	}

	// Add to class-relevant array
	if(!ActorsOfClass.Contains(ActorClass))
	{
		ActorsOfClass.Add(ActorClass, {InActor});
	} else
	{
		TArray<AActor*>& ClassActors = *ActorsOfClass.Find(ActorClass);
		ClassActors.Add(InActor);		
	}

	// Add to quest-relevant arrays
	if(!ActorClass->ImplementsInterface(UQuestable::StaticClass()))
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

TArray<AActor*> UCommonWorldUtils::Persistent_GetAllActorsOfClass(TSubclassOf<AActor> ActorClass)
{
	return Internal_GetAllActorsFromWorld(ActorClass, true);
}

TArray<AActor*> UCommonWorldUtils::CurrentStreamed_GetAllActorsOfClass(TSubclassOf<AActor> ActorClass)
{
	return Internal_GetAllActorsFromWorld(ActorClass, false);
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

TArray<AActor*> UCommonWorldUtils::Internal_GetAllActorsFromWorld(TSubclassOf<AActor> ActorClass, bool bInPersistentWorld)
{
	if(!ActorClass)
	{
		return {};
	}
	
	TArray<AActor*> OutActors;	
	for(const TTuple<UClass*, TArray<AActor*>> ClassEntry : ActorsOfClass)
	{
		const TSubclassOf<UObject> CurrClass = ClassEntry.Key;
		if(!CurrClass || !UKismetMathLibrary::ClassIsChildOf(CurrClass, ActorClass))
		{
			continue;
		}

		for(AActor* PotentialActor : ClassEntry.Value)
		{
			if(!PotentialActor)
			{
				continue;
			}
			
			if(bInPersistentWorld == PotentialActor->IsInPersistentLevel())
			{
				OutActors.Add(PotentialActor);
			}
		}
	}
	return OutActors;
}
