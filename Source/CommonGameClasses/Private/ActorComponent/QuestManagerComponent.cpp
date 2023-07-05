// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/QuestManagerComponent.h"
#include "API/Questable.h"
#include "Core/CommonGameMode.h"
#include "SMSystem/Public/SMUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Quest/QuestStateMachine.h"
#include "Quest/Transition_QuestSection.h"
#include "Utils/CommonCoreUtils.h"
#include "Utils/CommonWorldUtils.h"

UQuestManagerComponent::UQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	QuestIndexTicker = 0;
}

void UQuestManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	ACommonGameMode::EmptyQuestRelevantArray(this);
}

void UQuestManagerComponent::TryAddActorToActiveQuests(AActor* InActor)
{
	if(!InActor || !InActor->GetClass() || !InActor->GetClass()->ImplementsInterface(UQuestable::StaticClass()))
	{
		return;
	}
	
	if(UQuestManagerComponent* QuestManagerComponent = UCommonCoreUtils::GetQuestManager(InActor))
	{
		QuestManagerComponent->AddActorToActiveQuests(InActor);
	}
}

void UQuestManagerComponent::GiveQuestClassToPlayer(const UObject* WorldContextObject, TSubclassOf<UQuestStateMachine> QuestClass)
{
	if(UQuestManagerComponent* QuestManagerComponent = UCommonCoreUtils::GetQuestManager(WorldContextObject))
	{
		QuestManagerComponent->ActivateQuestInstance(QuestClass);
	}
}

void UQuestManagerComponent::AddActorToActiveQuests(AActor* InActor)
{
	TArray<int32> Keys;
	for (const auto ActiveQuest : ActiveQuests)
	{
		if(UQuestStateMachine* QuestStateMachine = ActiveQuest.Value)
		{
			QuestStateMachine->TryAddActorToQuest(InActor);
		}	
	}
}

void UQuestManagerComponent::ActivateQuestInstance(TSubclassOf<UQuestStateMachine> QuestClass)
{
	if(!QuestClass || !GetWorld() || HasQuestClassAlready(QuestClass))
	{
		return;
	}
	
	UQuestStateMachine* TempQuest = Cast<UQuestStateMachine>(USMBlueprintUtils::CreateStateMachineInstance(QuestClass, this));
	if(!TempQuest)
	{
		return;
	}
	
	const int32 TempQuestID = QuestIndexTicker++;
	TempQuest->QuestData.QuestID = TempQuestID;
	ActiveQuests.Add(TempQuestID, TempQuest);

	TempQuest->Start();
	for(UTransition_QuestSection* CurrTrans : TempQuest->QuestTransitions)
	{
		if(CurrTrans)
		{
			CurrTrans->OnQuestUpdated().AddUObject(this, &UQuestManagerComponent::HandleQuestUpdate);
		}		
	}
	QuestUpdate.Broadcast(FQuestUpdateEventPayload(TempQuest));
}

void UQuestManagerComponent::DeactivateAllQuests()
{
	TArray<int32> Keys;
	ActiveQuests.GetKeys(Keys);
	for(const int32 CurrKey : Keys)
	{
		DeactivateQuest(CurrKey);
	}
}

void UQuestManagerComponent::HandleQuestUpdate(const FQuestUpdateEventPayload& QuestObjectiveEventPayload)
{
	if(!QuestObjectiveEventPayload.UpdatedQuest)
		return;

	if(QuestObjectiveEventPayload.UpdatedQuest->IsQuestComplete())
	{
		ActiveQuests.Remove(QuestObjectiveEventPayload.UpdatedQuest->QuestData.QuestID);
	}
	
	QuestUpdate.Broadcast(QuestObjectiveEventPayload.UpdatedQuest);
}

UQuestStateMachine* UQuestManagerComponent::GetActiveQuest(int32 QuestID)
{
	return *ActiveQuests.Find(QuestID);
}

void UQuestManagerComponent::DeactivateQuest(int32 QuestID)
{
	if(const UQuestStateMachine* QuestInst = GetActiveQuest(QuestID))	{
		if(QuestInst->QuestData.QuestStateMachineInstance)
		{
			QuestInst->QuestData.QuestStateMachineInstance->Stop();
		}
		ActiveQuests.Remove(QuestID);
	}
}

bool UQuestManagerComponent::IsQuestComplete(int32 QuestID)
{
	if(const UQuestStateMachine* QuestInst = GetActiveQuest(QuestID))
	{
		if(QuestInst->QuestData.QuestStateMachineInstance)
		{
			return  QuestInst->QuestData.QuestStateMachineInstance->IsInEndState();
		}
	}
	return false;
}

bool UQuestManagerComponent::HasQuestClassAlready(TSubclassOf<UQuestStateMachine> QuestToCheck)
{
	TArray<int32> Keys;
	ActiveQuests.GetKeys(Keys);
	for(const int32 CurrKey : Keys)
	{
		if(const UQuestStateMachine* CurrQuest = *ActiveQuests.Find(CurrKey))
		{
			if(CurrQuest->GetClass() == QuestToCheck)
			{
				return true;
			}
		}
	}
	return false;
}
