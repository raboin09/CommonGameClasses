// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/CommonAICharacter.h"
#include "CommonAIController.generated.h"

UCLASS()
class COMMONGAMECLASSES_API ACommonAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACommonAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
private:
	UFUNCTION()
	virtual void Internal_OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	void InitAIComponents(UBehaviorTree* BehaviorTree);
	void InitPerceptionComponents();
	
	UPROPERTY(Transient, VisibleDefaultsOnly)
	class UAISenseConfig_Sight* Sight;
	UPROPERTY(Transient, VisibleDefaultsOnly)
	UAIPerceptionComponent* AIPerceptionComponent;
	
	int32 EnemyKeyID;
	int32 IsInCombatKeyID;
	int32 CanSeeEnemyID;
	
	UPROPERTY(Transient)
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;
	UPROPERTY(Transient)
	ACommonAICharacter* AIPawn;

};
