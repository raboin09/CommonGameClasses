// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCombatTypes.h"
#include "GameplayTagContainer.h"
#include "CommonResourceTypes.h"
#include "CommonQuestTypes.h"
#include "API/Ability/Ability.h"
#include "CommonEventDeclarations.generated.h"

///////////////////////////
// ABILITY ADDED
///////////////////////////
USTRUCT(BlueprintType)
struct FNewAbilityAddedPayload
{
	GENERATED_BODY()
	FNewAbilityAddedPayload(){}
	FNewAbilityAddedPayload(const TScriptInterface<IAbility> InAddedAbility) : AddedAbility(InAddedAbility)	{ }

	UPROPERTY()
	TScriptInterface<IAbility> AddedAbility;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewAbilityAdded, const FNewAbilityAddedPayload&, NewAbilityAddedPayload);

///////////////////////////
// ABILITY REMOVED
///////////////////////////
USTRUCT(BlueprintType)
struct FAbilityRemovedPayload
{
	GENERATED_BODY()
	FAbilityRemovedPayload(){}
	FAbilityRemovedPayload(const TScriptInterface<IAbility> InAbility) : RemovedAbility(InAbility) { }

	UPROPERTY()
	TScriptInterface<IAbility> RemovedAbility;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityRemoved, const FAbilityRemovedPayload&, AbilityRemovedPayload);

///////////////////////////
// NEW LEVEL LOADED
///////////////////////////
USTRUCT(BlueprintType)
struct FNewLevelLoadedEventPayload
{
	GENERATED_BODY()
	
	FNewLevelLoadedEventPayload(): NewStreamedWorld(nullptr)
	{
	}
	
	FNewLevelLoadedEventPayload(UWorld* InWorld): NewStreamedWorld(InWorld)
	{
	}

	UPROPERTY()
	UWorld* NewStreamedWorld;
};
DECLARE_EVENT_OneParam(ILevelLoadingManager, FNewLevelLoadedEvent, const FNewLevelLoadedEventPayload&);

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

	FTriggerEventPayload(bool bRunActivation) : bStartActivationImmediately(bRunActivation)
	{
		ActivationLevel = 0;
	}

	FTriggerEventPayload(bool bRunActivation, int32 ActivationLevel) : bStartActivationImmediately(bRunActivation), ActivationLevel(ActivationLevel) { }

	// Should Activation run immediately after this trigger event
	// Activation can happen after the trigger is released instead of trigger pulled (e.g. Bow), so it should be false
	// For other things (like hitscan guns and buffs/AoE abilities), should be true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStartActivationImmediately = true;

	// If a Montage handles activation instead of the Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMontageDrivesActivation = false;
	
	// How long has this trigger been pulled (charge/release guns and burst) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ActivationLevel = 0;
	
	// Optional location info to provide Activation mechanism (useful for things like throwing grenades where targeting happens during Trigger) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ActivationSourceLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	FAbilityActivationEventPayload() { }	
	FAbilityActivationEventPayload(bool bShouldStartCooldown) : bShouldStartCooldown(bShouldStartCooldown) { }

	// Some activation events don't start cooldowns until an external event happens (e.g. montage notifies)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldStartCooldown = true;
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
	
	FGameplayTagAddedEventPayload(const FGameplayTag& AddedTag) : AddedTag(AddedTag) { }
	
	UPROPERTY(BlueprintReadOnly)
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

	FGameplayTagRemovedEventPayload(const FGameplayTag& RemovedTag) : RemovedTag(RemovedTag) { }
	
	UPROPERTY(BlueprintReadOnly)
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
	FResourcePool NewWound = FResourcePool();
	UPROPERTY(BlueprintReadOnly)
	FResourcePool OldWound = FResourcePool();
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
	UObject* UpdatedQuest = nullptr;

	FQuestUpdateEventPayload(UObject* InUpdatedQuest)
	{
		UpdatedQuest = InUpdatedQuest;
	}

	FQuestUpdateEventPayload()
	{
		UpdatedQuest = nullptr;
	}
};
DECLARE_EVENT_OneParam(UQuestObjectiveComponent, FQuestUpdateEvent, const FQuestUpdateEventPayload&);

///////////////////////////////////////
// MANA UPDATE
///////////////////////////////////////

USTRUCT(BlueprintType)
struct FEnergyChangedEventPayload
{
	GENERATED_BODY()

	FEnergyChangedEventPayload()
	{
		CurrentEnergy = -1.f;
		MaxEnergy = -1.f;
	}

	FEnergyChangedEventPayload(const float InCurrentEnergy, const float InMaxEnergy)
	{
		CurrentEnergy = InCurrentEnergy;
		MaxEnergy = InMaxEnergy;
	}

	UPROPERTY(BlueprintReadOnly)
	float CurrentEnergy;
	UPROPERTY(BlueprintReadOnly)
	float MaxEnergy;
};
DECLARE_EVENT_OneParam(UManaComponent, FEnergyAmountChangedEvent, const FEnergyChangedEventPayload&);

///////////////////////////
// AMMO AMOUNT CHANGED
///////////////////////////
USTRUCT(BlueprintType)
struct FAmmoAmountChangedPayload
{
	GENERATED_BODY()

	FAmmoAmountChangedPayload(int32 InCurrAmmoInClip, int32 InClipCapacity, int32 InCurrentAmmo, int32 InMaxClips)
		: CurrentAmmoInClip(InCurrAmmoInClip), ClipCapacity(InClipCapacity),  CurrentAmmo(InCurrentAmmo), MaxAmmo(InMaxClips) {}

	FAmmoAmountChangedPayload(): CurrentAmmoInClip(0), ClipCapacity(0), CurrentAmmo(0), MaxAmmo(0)
	{
	}

	UPROPERTY()
	int32 CurrentAmmoInClip;
	UPROPERTY()
	int32 ClipCapacity;
	UPROPERTY()
	int32 CurrentAmmo;
	UPROPERTY()
	int32 MaxAmmo;
	
};
DECLARE_EVENT_OneParam(UAmmoComponent, FAmmoAmountChangedEvent, const FAmmoAmountChangedPayload&);

///////////////////////////
// AMMO AMOUNT CHANGED
///////////////////////////
USTRUCT(BlueprintType)
struct FOutOfAmmoEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
	bool bCompletelyOutOfAmmo = false;
};
DECLARE_EVENT_OneParam(UAmmoComponent, FOutOfAmmoEvent, const FOutOfAmmoEventPayload&);

///////////////////////////
// INTERACTION EVENT OCCURRED
///////////////////////////
USTRUCT(BlueprintType)
struct FInteractionStartedEventPayload
{
	GENERATED_BODY()

	FInteractionStartedEventPayload()
	{
		InstigatingActor = nullptr;
	}

	FInteractionStartedEventPayload(AActor* InInstigator, bool bIsStarting)
		: InstigatingActor(InInstigator), bIsStarting(bIsStarting) {	}
	
	UPROPERTY(BlueprintReadOnly)
	AActor* InstigatingActor = nullptr;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStarting = true;

};
DECLARE_EVENT_OneParam(UInteractionComponent, FInteractionStartedEvent, const FInteractionStartedEventPayload);

///////////////////////////
// INTERACTION EVENT OCCURRED
///////////////////////////
USTRUCT(BlueprintType)
struct FInteractionInitiatedEventPayload
{
	GENERATED_BODY()

	FInteractionInitiatedEventPayload()
	{
		InstigatingActor = nullptr;
	}

	FInteractionInitiatedEventPayload(AActor* InInstigator)
		: InstigatingActor(InInstigator) {	}
	
	UPROPERTY(BlueprintReadOnly)
	AActor* InstigatingActor = nullptr;

};
DECLARE_EVENT_OneParam(UInteractionComponent, FInteractionInitiatedEvent, const FInteractionInitiatedEventPayload);

///////////////////////////
// INTERACTION EVENT OCCURRED
///////////////////////////
USTRUCT(BlueprintType)
struct FInteractionOutlinedEventPayload
{
	GENERATED_BODY()

	FInteractionOutlinedEventPayload()
	{
		bShouldOutline = false;
	}

	FInteractionOutlinedEventPayload(bool bShouldOutline)
		: bShouldOutline(bShouldOutline) {	}
	
	UPROPERTY(BlueprintReadOnly)
	bool bShouldOutline = false;

};
DECLARE_EVENT_OneParam(UInteractionComponent, FInteractionOutlinedEvent, const FInteractionOutlinedEventPayload);
