// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CommonAIController.generated.h"

class UBotBehaviorComponent;
class UAISenseConfig_Sight;
class UBehaviorTreeComponent;
class UActorAssetManagerComponent;

UCLASS()
class COMMONGAMECLASSES_API ACommonAIController : public AAIController
{
	GENERATED_BODY()

	////////////////////////////////
	/// AIController overrides
	////////////////////////////////
public:
	ACommonAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;
	
	////////////////////////////////
	/// Common Functions
	////////////////////////////////
public:
	// Start a new behavior tree for the possessed bot
	UFUNCTION(BlueprintCallable)
	void InitAIBehavior(UBehaviorTree* BehaviorTree) const;
	
private:
	// Link the bot behavior component to the perception updates
	void InitPerceptionComponents();
	UFUNCTION()
	void HandleBehaviorTreeLoaded(TSoftObjectPtr<UBehaviorTree> LoadedTree);

	////////////////////////////////
	/// Common Variables
	////////////////////////////////
protected:
	UPROPERTY()
	TWeakObjectPtr<UBotBehaviorComponent> BotBehaviorComponent;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UAISenseConfig_Sight> Sight;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	UPROPERTY(Transient)
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(Transient)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
