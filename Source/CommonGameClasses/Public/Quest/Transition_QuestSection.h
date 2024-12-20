// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMTransitionInstance.h"
#include "Types/CommonEventDeclarations.h"
#include "Types/CommonQuestTypes.h"
#include "Transition_QuestSection.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UTransition_QuestSection : public USMTransitionInstance
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CUSTOM")
	FQuestSectionData QuestSectionData;
	int32 QuestID;

	void ActivateQuestObjectiveActor(AActor* InActor);
	FORCEINLINE FQuestUpdateEvent& OnQuestUpdated() { return QuestUpdated; }
	
protected:
	virtual bool CanEnterTransition_Implementation() const override;
	virtual void OnTransitionShutdown_Implementation() override;
	virtual void OnTransitionInitialized_Implementation() override;
	
	void ActivateQuest();
	void DeactivateQuest();

private:
	void ActivateAllObjectivesOfClass(UClass* ObjectiveClass);
	void DeactivateAllObjectivesOfClass(UClass* ObjectiveClass) const;
	void DeactivateQuestObjectiveActor(AActor* InActor) const;
	void ApplyQuestIncrementsForEvent(EQuestObjectiveAction Action, UClass* ValidClass);
	
	UFUNCTION()
	void HandleQuestEventTrigger(const FQuestObjectiveEventPayload& InEvent);

	FQuestUpdateEvent QuestUpdated;
};