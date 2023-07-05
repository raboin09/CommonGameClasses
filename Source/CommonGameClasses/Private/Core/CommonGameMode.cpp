// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonGameMode.h"
#include "ActorComponent/InteractionComponent.h"
#include "API/Questable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Types/CommonCoreTypes.h"

void ACommonGameMode::EmptyQuestRelevantArray(const UObject* ContextObject)
{
	ACommonGameMode* GameMode = Cast<ACommonGameMode>(UGameplayStatics::GetGameMode(ContextObject));
	if (!GameMode)
	{
		return;
	}
	GameMode->QuestRelevantActors.Empty();
}

void ACommonGameMode::TryAddActorToTrackedArrays(AActor* InActor)
{
	if(!InActor)
	{
		return;
	}
	
	ACommonGameMode* GameMode = Cast<ACommonGameMode>(UGameplayStatics::GetGameMode(InActor));
	if (!GameMode)
	{
		return;
	}
	
	// Add to affiliation-relevant arrays 
	if(const UInteractionComponent* InteractionComponent = InActor->FindComponentByClass<UInteractionComponent>())
	{
		const EAffiliation Affiliation = InteractionComponent->Affiliation;
		if(!GameMode->ActorsOfAffiliation.Contains(Affiliation))
		{
			const FWorldActorTrackerEntry& Entry =  FWorldActorTrackerEntry(InActor);
			GameMode->ActorsOfAffiliation.Add(Affiliation, Entry);
		} else
		{
			FWorldActorTrackerEntry& AffiliatedActors = *GameMode->ActorsOfAffiliation.Find(Affiliation);
			AffiliatedActors.Actors.Add(InActor);
		}
	}

	const TSubclassOf<AActor> ActorClass = InActor->GetClass();
	if(!ActorClass)
	{
		return;
	}
	
	// Add to class-relevant array
	if(!GameMode->ActorsOfClass.Contains(ActorClass))
	{
		GameMode->ActorsOfClass.Add(ActorClass, {InActor});
	} else
	{
		FWorldActorTrackerEntry& ClassActors = *GameMode->ActorsOfClass.Find(ActorClass);
		ClassActors.Actors.Add(InActor);
	}
	// Add to quest-relevant arrays
	if(!ActorClass->ImplementsInterface(UQuestable::StaticClass()))
	{
		return;
	}
	GameMode->QuestRelevantActors.AddUnique(InActor);
}

TArray<AActor*> ACommonGameMode::GetAllActorsOfClass_TrackedOnly(const UObject* ContextObject, TSubclassOf<AActor> ActorClass)
{
	if(!ActorClass)
	{
		return {};
	}

	ACommonGameMode* GameMode = Cast<ACommonGameMode>(UGameplayStatics::GetGameMode(ContextObject));
	if (!GameMode)
	{
		return {};
	}
	
	TArray<AActor*> OutActors;	
	for(const TTuple<UClass*, FWorldActorTrackerEntry> ClassEntry : GameMode->ActorsOfClass)
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

void ACommonGameMode::TryRemoveActorFromQuestableArray(AActor* InActor)
{
	if(!InActor || !InActor->GetClass() || !InActor->GetClass()->ImplementsInterface(UQuestable::StaticClass()))
	{
		return;
	}

	ACommonGameMode* GameMode = Cast<ACommonGameMode>(UGameplayStatics::GetGameMode(InActor));
	if (!GameMode)
	{
		return;
	}
	GameMode->QuestRelevantActors.Remove(InActor);
}
