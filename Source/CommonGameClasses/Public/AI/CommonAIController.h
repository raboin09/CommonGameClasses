// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CommonAIController.generated.h"

class UBotBehaviorComponent;
class UAISenseConfig_Sight;
class UBehaviorTreeComponent;

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

	////////////////////////////////
	/// Common Functions
	////////////////////////////////
public:
	// Start a new behavior tree for the possessed bot
	void InitAIBehavior(UBehaviorTree* BehaviorTree) const;
	
private:
	// Link the bot behavior component to the perception updates
	void InitPerceptionComponents(const UBotBehaviorComponent* BotBehaviorComponent);

	////////////////////////////////
	/// Common Variables
	////////////////////////////////
private:
	UPROPERTY(Transient, VisibleDefaultsOnly)
	UAISenseConfig_Sight* Sight;
	UPROPERTY(Transient, VisibleDefaultsOnly)
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(Transient)
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(Transient)
	UBehaviorTreeComponent* BehaviorTreeComponent;
};
