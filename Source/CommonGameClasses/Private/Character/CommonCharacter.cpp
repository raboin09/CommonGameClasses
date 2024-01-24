// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonCharacter.h"

#include "Ability/CommonAbility.h"
#include "ActorComponent/AbilityComponent.h"
#include "ActorComponent/ActorAssetManagerComponent.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/CommonCharacterMovementComponent.h"
#include "Core/ActorTrackingSubsystem.h"
#include "Utils/CommonCoreUtils.h"


ACommonCharacter ::ACommonCharacter(const FObjectInitializer& ObjectInitializer) : Super {ObjectInitializer.SetDefaultSubobjectClass<UCommonCharacterMovementComponent>(CharacterMovementComponentName)}
{
	PrimaryActorTick.bCanEverTick = true;

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	EffectContainerComponent = CreateDefaultSubobject<UEffectContainerComponent>(TEXT("EffectContainerComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	CharacterAnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>(TEXT("CharacterAnimationComponent"));
	ActorAssetManagerComponent = CreateDefaultSubobject<UActorAssetManagerComponent>(TEXT("ActorAssetManagerComponent"));
	CommonCharacterMovementComponent = Cast<UCommonCharacterMovementComponent>(GetCharacterMovement());
}

float ACommonCharacter::GetMaxSpeedRatio_Implementation() const
{
	return 1.f;
}

void ACommonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
	if(!AbilityComponent)
	{
		return;
	}
	
	for(auto Ability : DefaultAbilities)
	{
		if(!Ability.Value.IsValid())
		{
			FLoadedAbilityEvent LoadedClassEvent = FLoadedAbilityEvent();
			LoadedClassEvent.BindDynamic(this, &ThisClass::HandleDefaultAbilityLoaded);
			ActorAssetManagerComponent->K2_Async_LoadAbilityClass(Ability.Value, true, LoadedClassEvent);
		} else
		{
			AbilityComponent->AddAbilityFromClassInSlot(Ability.Value.Get(), Ability.Key);
			if(Ability.Key == CommonGameSlot::SlotMain)
			{
				AbilityComponent->SetCurrentEquippedSlot(CommonGameSlot::SlotMain);;
			}
		}
	}
}

void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	K2_HandleDeath();
}

void ACommonCharacter::SetMoveSpeedRatioIncrease(float Ratio)
{
	if(!CommonCharacterMovementComponent)
	{
		return;
	}
	CommonCharacterMovementComponent->SetWalkSpeedRatio(Ratio);
}

void ACommonCharacter::HandleDefaultAbilityLoaded(TSoftClassPtr<ACommonAbility> LoadedAbilityClass)
{
	for(auto Ability : DefaultAbilities)
	{
		if(Ability.Value == LoadedAbilityClass)
		{
			AbilityComponent->AddAbilityFromClassInSlot(LoadedAbilityClass.Get(), Ability.Key);
			if(Ability.Key == CommonGameSlot::SlotMain)
			{
				AbilityComponent->SetCurrentEquippedSlot(CommonGameSlot::SlotMain);;
			}
			return;
		}
	}
}
