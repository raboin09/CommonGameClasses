// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonCharacter.h"
#include "ActorComponent/GameplayTagComponent.h"

ACommonCharacter::ACommonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	ManaComponent = CreateDefaultSubobject<UEnergyComponent>(TEXT("ManaComponent"));
	EffectContainerComponent = CreateDefaultSubobject<UEffectContainerComponent>(TEXT("EffectContainerComponent"));
}

void ACommonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
}

void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACommonCharacter::HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload)
{
	K2_HandleTagAdded(TagAddedEventPayload);
}

void ACommonCharacter::HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload)
{
	K2_HandleTagRemoved(TagRemovedEventPayload);
}

void ACommonCharacter::K2_OnDeath_Implementation()
{
	// Events that should occur on death but before destruction
}
