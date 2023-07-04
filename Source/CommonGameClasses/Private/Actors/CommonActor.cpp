// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonActor.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Utils/CommonWorldUtils.h"

ACommonActor::ACommonActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
}

void ACommonActor::BeginPlay()
{
	Super::BeginPlay();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
	UCommonWorldUtils::TryAddActorToTrackedArrays(this);
}

void ACommonActor::HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload)
{
	K2_HandleTagAdded(TagAddedEventPayload);
}

void ACommonActor::HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload)
{
	K2_HandleTagRemoved(TagRemovedEventPayload);
}