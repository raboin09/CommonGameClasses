// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ActorTrackingSubsystem.h"

#include "ActorComponent/InteractionComponent.h"
#include "API/Questable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UActorTrackingSubsystem* UActorTrackingSubsystem::GetSubsystemFromWorld(const UWorld* World)
{
	if (IsValid(World))
	{
		return World->GetSubsystem<UActorTrackingSubsystem>();
	}
	return nullptr;
}

UActorTrackingSubsystem* UActorTrackingSubsystem::GetSubsystemFromActor(const AActor* Actor)
{
	if (IsValid(Actor))
	{
		return GetSubsystemFromWorld(Actor->GetWorld());
	}
	return nullptr;
}

void UActorTrackingSubsystem::EmptyQuestRelevantArray()
{
	QuestRelevantActors.Empty();
}

void UActorTrackingSubsystem::TryAddActorToTrackedArrays(AActor* InActor)
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
			const FWorldActorTrackerEntry& Entry =  FWorldActorTrackerEntry(InActor);
			ActorsOfAffiliation.Add(Affiliation, Entry);
		} else
		{
			FWorldActorTrackerEntry& AffiliatedActors = *ActorsOfAffiliation.Find(Affiliation);
			AffiliatedActors.Actors.Add(InActor);
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
		FWorldActorTrackerEntry& ClassActors = *ActorsOfClass.Find(ActorClass);
		ClassActors.Actors.Add(InActor);
	}
	// Add to quest-relevant arrays
	if(!ActorClass->ImplementsInterface(UQuestable::StaticClass()))
	{
		return;
	}
	QuestRelevantActors.AddUnique(InActor);
}

TArray<AActor*> UActorTrackingSubsystem::GetAllActorsOfClass_TrackedOnly(TSubclassOf<AActor> ActorClass)
{
	if(!ActorClass)
	{
		return {};
	}
	
	TArray<AActor*> OutActors;	
	for(const TTuple<UClass*, FWorldActorTrackerEntry>& ClassEntry : ActorsOfClass)
	{
		const TSubclassOf<UObject> CurrClass = ClassEntry.Key;
		if(!CurrClass || !UKismetMathLibrary::ClassIsChildOf(CurrClass, ActorClass))
		{
			continue;
		}

		OutActors.Append(ClassEntry.Value.Actors);
	}
	return OutActors;
}

void UActorTrackingSubsystem::TryRemoveActorFromQuestableArray(AActor* InActor)
{
	if(!InActor || !InActor->GetClass() || !InActor->GetClass()->ImplementsInterface(UQuestable::StaticClass()))
	{
		return;
	}

	UActorTrackingSubsystem* GameMode = Cast<UActorTrackingSubsystem>(UGameplayStatics::GetGameMode(InActor));
	if (!GameMode)
	{
		return;
	}
	GameMode->QuestRelevantActors.Remove(InActor);
}
