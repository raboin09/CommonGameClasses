// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagUtils.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UGameplayTagUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameplayTagUtils")
	static bool ComponentHasNameTag(UActorComponent* InComp, FName InTag);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameplayTagUtils")
	static bool ActorHasAnyGameplayTags(AActor* InActor, TArray<FGameplayTag> InTags, bool bExact = false);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|GameplayTagUtils")
	static bool ActorHasGameplayTag(AActor* InActor, FGameplayTag InTag, bool bExact = false);
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagUtils")
	static void AddTagToActor(AActor* InActor, FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagUtils")
	static void AddTagsToActor(AActor* InActor, TArray<FGameplayTag> InTags);
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagUtils")
	static void RemoveTagsFromActor(AActor* InActor, TArray<FGameplayTag> InTags);
	UFUNCTION(BlueprintCallable, Category="COMMON|GameplayTagUtils")
	static void RemoveTagFromActor(AActor* InActor, FGameplayTag InTag);
};
