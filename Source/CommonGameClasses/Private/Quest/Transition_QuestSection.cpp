// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/Transition_QuestSection.h"

#include "API/Questable.h"
#include "Character/CommonPlayerCharacter.h"
#include "Systems/ActorTrackingSubsystem.h"
#include "Quest/QuestStateMachine.h"
#include "Utils/CommonCoreUtils.h"

bool UTransition_QuestSection::CanEnterTransition_Implementation() const
{
	return QuestSectionData.IsQuestSectionDone();
}

void UTransition_QuestSection::OnTransitionShutdown_Implementation()
{
	QuestUpdated.RemoveAll(this);
	DeactivateQuest();
}

void UTransition_QuestSection::OnTransitionInitialized_Implementation()
{
	ActivateQuest();
}

void UTransition_QuestSection::ActivateQuest()
{
	for(TSoftClassPtr<AActor> CurrClass : QuestSectionData.TargetObjectiveClasses)
	{
		ActivateAllObjectivesOfClass(CurrClass.Get());
	}
}

void UTransition_QuestSection::DeactivateQuest()
{
	QuestUpdated.RemoveAll(this);
	for(TSoftClassPtr<AActor> CurrClass : QuestSectionData.TargetObjectiveClasses)
	{
		DeactivateAllObjectivesOfClass(CurrClass.Get());
	}
}

void UTransition_QuestSection::ActivateAllObjectivesOfClass(UClass* ObjectiveClass)
{
	if(!ObjectiveClass)
	{
		return;
	}

	if(UActorTrackingSubsystem* ActorTrackingSubsystem = UCommonCoreUtils::GetActorTrackingSubsystem(this))
	{
		for(TWeakObjectPtr<AActor> CurrActor : ActorTrackingSubsystem->QuestRelevantActors)
		{
			if(CurrActor.IsStale())
			{
				continue;
			}
		
			if(QuestSectionData.ObjectiveTag.IsNone() || CurrActor->ActorHasTag(QuestSectionData.ObjectiveTag))
			{
				ActivateQuestObjectiveActor(CurrActor.Get());	
			}
		}
	}
	

}

void UTransition_QuestSection::ActivateQuestObjectiveActor(AActor* InActor)
{
	if(!InActor)
	{
		return;
	}

	if(IQuestable* CastedActor = Cast<IQuestable>(InActor))
	{
		CastedActor->OnQuestObjectiveEvent().AddUObject(this, &ThisClass::HandleQuestEventTrigger);
	}
}

void UTransition_QuestSection::DeactivateAllObjectivesOfClass(UClass* ObjectiveClass) const
{
	if(ObjectiveClass)
	{
		const ACommonPlayerCharacter* PlayerCharacter = UCommonCoreUtils::GetCommonPlayerCharacter(this);
		if (!PlayerCharacter)
		{
			return;
		}

		UActorTrackingSubsystem* ActorTrackingSubsystem = UActorTrackingSubsystem::GetSubsystemFromWorld(PlayerCharacter->GetWorld());
		
		for(TWeakObjectPtr<AActor> CurrActor : ActorTrackingSubsystem->QuestRelevantActors)
		{
			DeactivateQuestObjectiveActor(CurrActor.Get());
		}
	}
}

void UTransition_QuestSection::DeactivateQuestObjectiveActor(AActor* InActor) const
{
	if(IQuestable* CastedActor = Cast<IQuestable>(InActor))
	{
		CastedActor->OnQuestObjectiveEvent().RemoveAll(this);
	}
}

void UTransition_QuestSection::HandleQuestEventTrigger(const FQuestObjectiveEventPayload& InEvent)
{
	if(InEvent.EventObjective.IsValid() && QuestSectionData.ValidObjectiveActions.Contains(InEvent.EventAction))
	{
		ApplyQuestIncrementsForEvent(InEvent.EventAction, InEvent.EventObjective->GetClass());
	}
}

void UTransition_QuestSection::ApplyQuestIncrementsForEvent(EQuestObjectiveAction Action, UClass* ValidClass)
{
	if(QuestSectionData.TargetObjectiveClasses.Contains(ValidClass) && QuestSectionData.ValidObjectiveActions.Contains(Action))
	{
		if(!QuestSectionData.IsQuestSectionDone())
		{
			QuestSectionData.NumCurrentActions++;
		}
		
		if(UQuestStateMachine* CurrQuest = Cast<UQuestStateMachine>(GetStateMachineInstance(true)))
		{
			const TWeakObjectPtr<UQuestStateMachine> CurrQuestWeak = CurrQuest;
			QuestUpdated.Broadcast(CurrQuestWeak);
		}
	}
}