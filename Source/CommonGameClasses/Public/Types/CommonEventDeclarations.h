// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCombatTypes.h"
#include "GameplayTagContainer.h"
#include "CommonResourceTypes.h"
#include "CommonQuestTypes.h"
#include "CommonEventDeclarations.generated.h"

class IAbility;

///////////////////////////
// Ability equipped
///////////////////////////
USTRUCT(BlueprintType)
struct FAbilityFinishedEquipPayload
{
	GENERATED_BODY()
	FAbilityFinishedEquipPayload(){}
	FAbilityFinishedEquipPayload(const TScriptInterface<IAbility> InEquippedAbility) : EquippedAbility(InEquippedAbility)	{ }
	
	UPROPERTY()
	TScriptInterface<IAbility> EquippedAbility;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityFinishedEquip, const FAbilityFinishedEquipPayload&, AbilityFinishedEquipPayload);

USTRUCT(BlueprintType)
struct FNewAbilityEquippedPayload
{
	GENERATED_BODY()
	FNewAbilityEquippedPayload(){}
	FNewAbilityEquippedPayload(const TScriptInterface<IAbility> InEquippedAbility) : NewEquippedAbility(InEquippedAbility)	{ }
	
	UPROPERTY()
	TScriptInterface<IAbility> NewEquippedAbility;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewAbilityEquipped, const FNewAbilityEquippedPayload&, NewAbilityEquippedPayload);

///////////////////////////
// Ability added
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
// Ability removed
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
// New level added
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayTagAddedEvent, const FGameplayTagAddedEventPayload&, GameplayTagAddedEventPayload);

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayTagRemovedEvent, const FGameplayTagRemovedEventPayload&, GameplayTagRemovedEventPayload);

///////////////////////////
// Death event
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDeath, const FActorDeathEventPayload&, ActorDeathEventPayload);

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxWoundsChanged, const FMaxWoundsEventPayload&, MaxWoundsEventPayload);

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWoundHealthChanged, const FCurrentWoundEventPayload&, CurrentWoundEventPayload);

///////////////////////////////////////
// Quest Event
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
// Quest Update
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnergyAmountChangedEvent, const FEnergyChangedEventPayload&, EnergyChangedEventPayload);

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoAmountChangedEvent, const FAmmoAmountChangedPayload&, AmmoAmountChangedPayload);

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOutOfAmmoEvent, const FOutOfAmmoEventPayload&, OutOfAmmoEventPayload);

//////////////////////////////
// Interaction event occurred
//////////////////////////////
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

//////////////////////////////
// Interaction event occurred
//////////////////////////////
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

/////////////////////////////////////
// Interaction outline event occurred
/////////////////////////////////////
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