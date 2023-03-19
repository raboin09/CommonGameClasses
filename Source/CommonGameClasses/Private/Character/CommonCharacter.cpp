// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonCharacter.h"

ACommonCharacter::ACommonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
}

void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
}

void ACommonCharacter::K2_OnDeath_Implementation()
{
	// Events that should occur on death but before destruction
}
