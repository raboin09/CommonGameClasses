// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonCharacter.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "Core/ActorTrackingSubsystem.h"
#include "Utils/CommonCoreUtils.h"


ACommonCharacter::ACommonCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	EffectContainerComponent = CreateDefaultSubobject<UEffectContainerComponent>(TEXT("EffectContainerComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	CharacterAnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>(TEXT("CharacterAnimationComponent"));
}

void ACommonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
}

void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(!AbilityComponent)
	{
		return;
	}
	
	for(auto Ability : DefaultAbilities)
	{
		AbilityComponent->AddAbilityFromClassInSlot(Ability.Value, Ability.Key);		
	}
	
	if(UActorTrackingSubsystem* ActorTrackingSubsystem = UCommonCoreUtils::GetActorTrackingSubsystem(this))
	{
		ActorTrackingSubsystem->TryAddActorToTrackedArrays(this);	
	}
}

void ACommonCharacter::HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload)
{
	K2_HandleTagAdded(TagAddedEventPayload);
}

void ACommonCharacter::HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload)
{
	K2_HandleTagRemoved(TagRemovedEventPayload);
}

void ACommonCharacter::HandleDeath()
{
	if (!IsAlive())
	{
		return;
	}
	UGameplayTagComponent::AddTagToActor(this, CommonGameState::Dead);
	DetachFromControllerPendingDestroy();
	AbilityComponent->DestroyAbilities();
	K2_OnDeath();
}