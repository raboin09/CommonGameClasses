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
	float CooldownLeft;
	UPROPERTY()
	float TotalCooldown;
};

///////////////////////////////////////
// Cooldown Started
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FCooldownStartedEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	float CooldownLength;
};

///////////////////////////////////////
// Cooldown Ended
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FCooldownEndedEventPayload
{
	GENERATED_BODY()
};

///////////////////////////////////////
// Trigger Activated
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FTriggerActivatedEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	bool bDoesTriggerControlActivationDirectly;
};

///////////////////////////////////////
// Trigger Deactivated
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FTriggerDeactivatedEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	bool bDoesTriggerControlActivationDirectly;
};

///////////////////////////////////////
// Activation function started
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FAbilityActivationEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	bool bStartCooldownImmediately = true;
};
DECLARE_EVENT_OneParam(IActivationMechanism, FAbilityActivationEvent, const FAbilityActivationEventPayload&);

///////////////////////////////////////
// Activation function ended
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FAbilityDeactivationEventPayload
{
	GENERATED_BODY()
};
DECLARE_EVENT_OneParam(IActivationMechanism, FAbilityDeactivationEvent, const FAbilityDeactivationEventPayload&);

///////////////////////////////////////
// Gameplay tag added
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FGameplayTagAddedEventPayload
{
	GENERATED_BODY()

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
	FHitResult HitResult;
	UPROPERTY()
	FDamageHitReactEvent HitReactEvent;
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
	float NewMaxWounds;
	UPROPERTY()
	float OldMaxWounds;
	UPROPERTY()
	float Delta;
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
	FWound NewWound;
	UPROPERTY(BlueprintReadOnly)
	FWound OldWound;
	UPROPERTY()
	int32 MaxWounds;
	UPROPERTY()
	float Delta;
	UPROPERTY()
	float Percentage;
	UPROPERTY()
	bool bWasDamage;
	UPROPERTY()
	bool bNaturalChange;
	UPROPERTY()
	FDamageHitReactEvent DamageHitReactEvent;
	UPROPERTY()
	AActor* ReceivingActor;
	UPROPERTY()
	AActor* InstigatingActor;
};
DECLARE_EVENT_OneParam(UHealthComponent, FCurrentWoundHealthChanged, const FCurrentWoundEventPayload&);

///////////////////////////////////////
// PLAYER IN COMBAT CHANGED EVENT
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FCharacterInCombatChangedPayload
{
	GENERATED_BODY()
	FCharacterInCombatChangedPayload()
	{
		bIsInCombat = false;
		DamageCauser = nullptr;
	}
	FCharacterInCombatChangedPayload(bool bInIsInCombat, AActor* InDamageCauser) : bIsInCombat(bInIsInCombat), DamageCauser(InDamageCauser) { }

	UPROPERTY()
	bool bIsInCombat;
	UPROPERTY()
	AActor* DamageCauser;
};

///////////////////////////////////////
// QUEST EVENT
///////////////////////////////////////
USTRUCT(BlueprintType)
struct FQuestObjectiveEventPayload
{
	GENERATED_BODY()
	
	// The overlapped actor, quest objective, or killed AI character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* EventObjective;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestObjectiveAction EventAction;

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
	UQuestStateMachine* UpdatedQuest;

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