﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/CommonEventDeclarations.h"
#include "Components/ActorComponent.h"
#include "GameplayTagComponent.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UGameplayTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	////////////////////////////////////////////
	// Static Functions
	////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameplayTagComponent")
	static bool ActorHasAnyGameplayTags(AActor* InActor, TArray<FGameplayTag> InTags, bool bExact = false);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameplayTagComponent")
	static bool ActorHasAllGameplayTags(AActor* InActor, TArray<FGameplayTag> InTags, bool bExact = false);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameplayTagComponent")
	static bool ActorHasGameplayTag(AActor* InActor, FGameplayTag InTag, bool bExact = false);
	
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagComponent")
	static void AddTagToActor(AActor* InActor, const FGameplayTag& TagToAdd, UGameplayTagComponent* GameplayTagComponent = nullptr);
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagComponent")
	static void AddTagsToActor(AActor* InActor, const TArray<FGameplayTag>& InTags);
	
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagComponent")
	static void RemoveTagFromActor(AActor* InActor, const FGameplayTag& TagToRemove, UGameplayTagComponent* GameplayTagComponent = nullptr);
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagComponent")
	static void RemoveTagsFromActor(AActor* InActor, const TArray<FGameplayTag>& InTags);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameplayTagComponent")
	static UGameplayTagComponent* GetGameplayTagComponentFromActor(AActor* InActor);

	// Just a catch-all for all non-GameplayTag tag checks
	static bool ComponentHasNameTag(UActorComponent* InComp, FName InTag);
	
	////////////////////////////////////////////
	// GameplayTagComponent Definitions
	////////////////////////////////////////////
	
	FORCEINLINE FGameplayTagAddedEvent& OnGameplayTagAdded() { return GameplayTagAddedEvent; }
	FORCEINLINE FGameplayTagRemovedEvent& OnGameplayTagRemoved() { return GameplayTagRemovedEvent; }
	FORCEINLINE FGameplayTagContainer& GetTagContainer() { return GameplayTagContainer; }
	
	void RemoveTag(const FGameplayTag& TagToRemove);
	void AddTag(const FGameplayTag& TagToAdd);
	virtual void BeginPlay() override;
	
private:	
	FGameplayTagContainer GameplayTagContainer;

	FGameplayTagAddedEvent GameplayTagAddedEvent;
	FGameplayTagRemovedEvent GameplayTagRemovedEvent;
};
