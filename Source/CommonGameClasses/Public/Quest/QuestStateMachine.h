// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMInstance.h"
#include "Types/CommonQuestTypes.h"
#include "QuestStateMachine.generated.h"

class UTransition_QuestSection;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UQuestStateMachine : public USMInstance
{
	GENERATED_BODY()
public:
	UQuestStateMachine();
	virtual void Initialize(UObject* Context) override;

	void TryAddActorToQuest(AActor* InActor);
	bool IsQuestComplete() const;
	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FQuestData QuestData;

	UPROPERTY()
	TArray<UTransition_QuestSection*> QuestTransitions;
};