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

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("COMMON"), PrioritizeCategories = ("COMMON"), HideCategories=(COMMON_IGNORE_CATEGORIES))
class COMMONGAMECLASSES_API ACommonActor : public AActor, public ITaggable, public ISavableActor
{
	GENERATED_BODY()

public:
	ACommonActor();

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin ISavableActor Interface
	virtual void PostActorLoadedFromSave(const FCommonActorSaveData& InActorSaveData) override;
	//~ End ISavableActor Interface

	//~ Begin ITaggable Interface
	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload) override;
	//~ End ITaggable Interface
	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_OnTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_OnTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COMMON")
	TArray<FGameplayTag> DefaultGameplayTags;

	UPROPERTY()
	TObjectPtr<UGameplayTagComponent> GameplayTagComponent;
};
