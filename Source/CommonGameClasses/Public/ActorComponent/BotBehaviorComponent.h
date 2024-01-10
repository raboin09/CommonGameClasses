// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BotBehaviorComponent.generated.h"

class UBehaviorTree;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UBotBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

	////////////////////////////////
	/// ActorComponent overrides
	////////////////////////////////
public:
	UBotBehaviorComponent();

	////////////////////////////////
	/// Events
	////////////////////////////////
public:
	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	////////////////////////////////
	/// Common Variables
	////////////////////////////////
protected:
	UPROPERTY(EditAnywhere, Category="CUSTOM|Defaults")
	UBehaviorTree* DefaultBehaviorTree;

	/////////////////////////////////
	/// FORCEINLINE
	/////////////////////////////////
public:
	FORCEINLINE virtual UBehaviorTree* GetDefaultBehavior() const { return DefaultBehaviorTree; }
};
