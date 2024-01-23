// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/ActorAssetManagerComponent.h"
#include "API/Taggable.h"
#include "GameFramework/Actor.h"
#include "Types/CommonEventDeclarations.h"
#include "CommonActor.generated.h"

class UActorAssetManagerComponent;
struct FGameplayTag;
class UGameplayTagComponent;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM"), PrioritizeCategories = "CUSTOM")
class COMMONGAMECLASSES_API ACommonActor : public AActor, public ITaggable
{
	GENERATED_BODY()

public:
	ACommonActor();
	
	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	TArray<FGameplayTag> DefaultGameplayTags;

	UPROPERTY(BlueprintReadOnly)
	UActorAssetManagerComponent* ActorAssetManagerComponent;
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;
};
