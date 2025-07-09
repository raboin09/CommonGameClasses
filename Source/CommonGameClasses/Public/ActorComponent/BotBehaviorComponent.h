// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/ActorComponent.h"
#include "BotBehaviorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Variable", "Sockets", "Tags", "ComponentReplication", "Component Replication", "Cooking", "Replication", "AssetUserData", "Asset User Data", "Navigation"))
class COMMONGAMECLASSES_API UBotBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

	////////////////////////////////
	/// ActorComponent overrides
	////////////////////////////////
public:
	UBotBehaviorComponent();
	virtual void BeginPlay() override;

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
	UPROPERTY(EditAnywhere, Category="COMMON|Defaults")
	TSoftObjectPtr<UBehaviorTree> DefaultBehaviorTree;

	/////////////////////////////////
	/// FORCEINLINE
	/////////////////////////////////
public:
	FORCEINLINE virtual TSoftObjectPtr<UBehaviorTree> GetDefaultBehavior() const { return DefaultBehaviorTree; }
};
