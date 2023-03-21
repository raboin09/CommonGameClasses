// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HealthTypes.h"
#include "QuestTypes.h"
#include "EventDeclarations.generated.h"

///////////////////////////////////////
// Cooldown Tick
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FCooldownTickedEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	float CooldownLeft = 0.f;
	UPROPERTY()
	float TotalCooldown = 0.f;
};
DECLARE_EVENT_OneParam(ICooldownMechanism, FCooldownTickedEvent, const FCooldownTickedEventPayload&)

///////////////////////////////////////
// Cooldown Started
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FCooldownStartedEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	float CooldownLength = -1;
};
DECLARE_EVENT_OneParam(ICooldownMechanism, FCooldownStartedEvent, const FCooldownStartedEventPayload&)

///////////////////////////////////////
// Cooldown Ended
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FCooldownEndedEventPayload
{
	GENERATED_BODY()

	FCooldownEndedEventPayload()
	{
		
	}
};
DECLARE_EVENT_OneParam(ICooldownMechanism, FCooldownEndedEvent, const FCooldownEndedEventPayload&)

///////////////////////////////////////
// Trigger Activated
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FTriggerEventPayload
{
	GENERATED_BODY()

	FTriggerEventPayload()
	{
		ActivationLevel = 1;
	}

	FTriggerEventPayload(int32 ActivationLevel) : ActivationLevel(ActivationLevel) { }
	
	// How long has this trigger been pulled (charge/release guns and burst) 
	UPROPERTY()
	int32 ActivationLevel;
	// Optional location info to provide Activation mechanism (useful for things like throwing grenades where targeting happens during Trigger) 
	UPROPERTY()
	FVector ActivationSourceLocation = FVector::ZeroVector;
	// Optional location info to provide Activation mechanism (useful for things like throwing grenades where targeting happens during Trigger) 
	UPROPERTY()
	FVector ActivationDestinationLocation = FVector::ZeroVector;
};
DECLARE_EVENT_OneParam(ITriggerMechanism, FTriggerEvent, const FTriggerEventPayload&);

///////////////////////////////////////
// Activation function started
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FAbilityActivationEventPayload
{
	GENERATED_BODY()

	FAbilityActivationEventPayload()
	{
		
	}
};
DECLARE_EVENT_OneParam(IActivationMechanism, FAbilityActivationEvent, const FAbilityActivationEventPayload&);

///////////////////////////////////////
// Activation function ended
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FAbilityDeactivationEventPayload
{
	GENERATED_BODY()

	FAbilityDeactivationEventPayload()
	{
		
	}
};
DECLARE_EVENT_OneParam(IActivationMechanism, FAbilityDeactivationEvent, const FAbilityDeactivationEventPayload&);

///////////////////////////////////////
// Gameplay tag added
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FGameplayTagAddedEventPayload
{
	GENERATED_BODY()

	FGameplayTagAddedEventPayload()
	{
		AddedTag = FGameplayTag::EmptyTag;
	}

	UPROPERTY()
	FGameplayTag AddedTag;
};
DECLARE_EVENT_OneParam(UGameplayTagComponent, FGameplayTagAddedEvent, const FGameplayTagAddedEventPayload&)

///////////////////////////////////////
// Gameplay tag removed
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FGameplayTagRemovedEventPayload
{
	GENERATED_BODY()

	FGameplayTagRemovedEventPayload()
	{
		RemovedTag = FGameplayTag::EmptyTag;
	}
	
	UPROPERTY()
	FGameplayTag RemovedTag;
};
DECLARE_EVENT_OneParam(UGameplayTagComponent, FGameplayTagRemovedEvent, const FGameplayTagRemovedEventPayload&)

///////////////////////////
// DEATH EVENT
///////////////////////////
USTRUCT(BlueprintType)
struct FActorDeathEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	float DyingDamage = 0.f;
	UPROPERTY()
	AActor* DyingActor = nullptr;
	UPROPERTY()
	AActor* KillingActor = nullptr;
	UPROPERTY()
	FHitResult HitResult = FHitResult();
	UPROPERTY()
	FDamageHitReactEvent HitReactEvent = FDamageHitReactEvent();
};
DECLARE_EVENT_OneParam(UHealthComponent, FActorDeath, const FActorDeathEventPayload&);

///////////////////////////
// MAX WOUNDS EVENT
///////////////////////////
USTRUCT(BlueprintType)
struct FMaxWoundsEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	float NewMaxWounds = 1.f;
	UPROPERTY()
	float OldMaxWounds = 1.f;
	UPROPERTY()
	float Delta = 0.f;
};
DECLARE_EVENT_OneParam(UHealthComponent, FMaxWoundsChanged, const FMaxWoundsEventPayload&);

///////////////////////////
// CURRENT WOUND EVENT
///////////////////////////
USTRUCT(BlueprintType)
struct FCurrentWoundEventPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FWound NewWound = FWound();
	UPROPERTY(BlueprintReadOnly)
	FWound OldWound = FWound();
	UPROPERTY()
	int32 MaxWounds = 1;
	UPROPERTY()
	float Delta = 0.f;
	UPROPERTY()
	float Percentage = 0.f;
	UPROPERTY()
	bool bWasDamage = false;
	UPROPERTY()
	bool bNaturalChange = false;
	UPROPERTY()
	FDamageHitReactEvent DamageHitReactEvent = FDamageHitReactEvent();
	UPROPERTY()
	AActor* ReceivingActor = nullptr;
	UPROPERTY()
	AActor* InstigatingActor = nullptr;
};
DECLARE_EVENT_OneParam(UHealthComponent, FCurrentWoundHealthChanged, const FCurrentWoundEventPayload&);

///////////////////////////////////////
// QUEST EVENT
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FQuestObjectiveEventPayload
{
	GENERATED_BODY()
	
	// The overlapped actor, quest objective, or killed AI character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* EventObjective = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestObjectiveAction EventAction = EQuestObjectiveAction::None;

	FQuestObjectiveEventPayload(AActor* InObjective, EQuestObjectiveAction InAction)
	{
		EventObjective = InObjective;
		EventAction = InAction;
	}

	FQuestObjectiveEventPayload()
	{
		EventObjective = nullptr;
		EventAction = EQuestObjectiveAction::None;
	}
};
DECLARE_EVENT_OneParam(UQuestObjectiveComponent, FQuestObjectiveEvent, const FQuestObjectiveEventPayload&);

///////////////////////////////////////
// QUEST UPDATE
///////////////////////////////////////

USTRUCT(BlueprintType)
struct FQuestUpdateEventPayload
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UQuestStateMachine* UpdatedQuest = nullptr;

	FQuestUpdateEventPayload(UQuestStateMachine* InUpdatedQuest)
	{
		UpdatedQuest = InUpdatedQuest;
	}

	FQuestUpdateEventPayload()
	{
		UpdatedQuest = nullptr;
	}
};
DECLARE_EVENT_OneParam(UQuestObjectiveComponent, FQuestUpdateEvent, const FQuestUpdateEventPayload&);