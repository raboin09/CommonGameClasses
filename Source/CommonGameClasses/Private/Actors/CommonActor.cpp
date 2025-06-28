// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonActor.h"

#include "ActorComponent/GameplayTagComponent.h"
#include "Core/ActorSystems/ActorTrackingSubsystem.h"
#include "Utils/CommonCoreUtils.h"

ACommonActor::ACommonActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
}

void ACommonActor::BeginPlay()
{
	Super::BeginPlay();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
	if(UActorTrackingSubsystem* ActorTrackingSubsystem = UCommonCoreUtils::GetActorTrackingSubsystem(this))
	{
		ActorTrackingSubsystem->TryAddActorToTrackedArrays(this);	
	}
}

void ACommonActor::HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload)
{
	BPI_HandleTagAdded(TagAddedEventPayload);
}

void ACommonActor::HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload)
{
	BPI_HandleTagRemoved(TagRemovedEventPayload);
}