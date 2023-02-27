// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonActor.h"

ACommonActor::ACommonActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACommonActor::BeginPlay()
{
	Super::BeginPlay();
	Internal_AddDefaultTagsToContainer();
}

void ACommonActor::HandleTagChanged(const FGameplayTag& ChangedTag, bool bAdded)
{
	if(bAdded)
	{
		K2_HandleTagAdded(ChangedTag);
	}
	else
	{
		K2_HandleTagRemoved(ChangedTag);
	}
}

void ACommonActor::K2_HandleTagRemoved_Implementation(const FGameplayTag& AddedTag)
{
	
}

void ACommonActor::K2_HandleTagAdded_Implementation(const FGameplayTag& AddedTag)
{
	
}

void ACommonActor::Internal_AddDefaultTagsToContainer()
{
	for(const FGameplayTag CurrentTag : DefaultGameplayTags)
	{
		GameplayTagContainer.AddTag(CurrentTag);
	}
}