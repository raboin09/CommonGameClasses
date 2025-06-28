// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Taggable.h"
#include "API/Core/SavableActor.h"
#include "GameFramework/Actor.h"
#include "Types/CommonEventDeclarations.h"
#include "CommonActor.generated.h"

struct FGameplayTag;
class UGameplayTagComponent;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM"), PrioritizeCategories = "CUSTOM")
class COMMONGAMECLASSES_API ACommonActor : public AActor, public ITaggable, public ISavableActor
{
	GENERATED_BODY()

public:
	ACommonActor();
	
	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	TArray<FGameplayTag> DefaultGameplayTags;

	UPROPERTY()
	TObjectPtr<UGameplayTagComponent> GameplayTagComponent;
};
