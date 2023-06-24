// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest/QuestStateMachine.h"
#include "Types/CommonEventDeclarations.h"
#include "QuestManagerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuestManagerComponent();

	static void TryAddActorToActiveQuests(AActor* InActor);
	UFUNCTION(BlueprintCallable, Category="COMMON|QuestManager")
	static void GiveQuestClassToPlayer(const UObject* WorldContextObject, TSubclassOf<UQuestStateMachine> QuestClass);

	void AddActorToActiveQuests(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category="COMMON|QuestManager")
	void ActivateQuestInstance(TSubclassOf<UQuestStateMachine> QuestClass);
	UFUNCTION(BlueprintCallable, Category="COMMON|QuestManager")
	void DeactivateAllQuests();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|QuestManager")
	UQuestStateMachine* GetActiveQuest(int32 QuestID);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|QuestManager")
	int32 GetNumberOfQuests() const { return ActiveQuests.Num(); }
	UFUNCTION(BlueprintCallable, Category="COMMON|QuestManager")
	void DeactivateQuest(int32 QuestID);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|QuestManager")
	bool IsQuestComplete(int32 QuestID);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|QuestManager")
	bool HasQuestClassAlready(TSubclassOf<UQuestStateMachine> QuestToCheck);

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void HandleQuestUpdate(const FQuestUpdateEventPayload& QuestObjectiveEventPayload);
	
private:

	UPROPERTY()
	TMap<int32, UQuestStateMachine*> ActiveQuests;
	int32 QuestIndexTicker;
	FQuestUpdateEvent QuestUpdate;

public:
	
	FORCEINLINE int32 GenerateNextQuestID() { return ++QuestIndexTicker; }
	FORCEINLINE TMap<int32, UQuestStateMachine*> GetQuestMap() const { return ActiveQuests; }
	FORCEINLINE FQuestUpdateEvent& OnQuestUpdate() { return QuestUpdate; }
};
