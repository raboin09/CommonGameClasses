// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCombatTypes.h"
#include "CommonCoreTypes.h"
#include "GameplayTagContainer.h"
#include "CommonResourceTypes.h"
#include "CommonTagTypes.h"
#include "UObject/WeakInterfacePtr.h"
#include "CommonEventDeclarations.generated.h"

class ACommonCharacter;
class ACommonActor;
class ACommonAbility;
class UBehaviorTree;
class IAbility;

///////////////////////////
// Montage Ended
///////////////////////////
USTRUCT(BlueprintType)
struct FCharacterMontageEndedPayload
{
	GENERATED_BODY()
	FCharacterMontageEndedPayload()
	{
		EndedMontage = nullptr;
		UpcomingMontage = nullptr;
		bInterrupted = false;
	}
	
	FCharacterMontageEndedPayload(const TObjectPtr<UAnimMontage> InMontage, const TObjectPtr<UAnimMontage> InUpcomingMontage, const bool bInInterrupted)
	{
		EndedMontage = InMontage;
		UpcomingMontage = InUpcomingMontage;
		bInterrupted = bInInterrupted;
	}

	UPROPERTY()
	TWeakObjectPtr<UAnimMontage> UpcomingMontage;
	UPROPERTY()
	TWeakObjectPtr<UAnimMontage> EndedMontage;
	UPROPERTY()
	bool bInterrupted;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterMontageEnded, const FCharacterMontageEndedPayload&, CharacterMontageEndedPayload);

///////////////////////////
// Ragdoll Event
///////////////////////////
USTRUCT(BlueprintType)
struct FCharacterRagdollEventPayload
{
	GENERATED_BODY()
	FCharacterRagdollEventPayload(): bIsRagdolling(false)
	{
	}

	FCharacterRagdollEventPayload(bool bInIsRagdolling)
	{
		bIsRagdolling = bInIsRagdolling;
	}

	UPROPERTY(BlueprintReadOnly)
	bool bIsRagdolling;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterRagdollingEvent, const FCharacterRagdollEventPayload&, CharacterRagdollingEventPayload);


///////////////////////////
// Camera type changed
///////////////////////////
USTRUCT(BlueprintType)
struct FCameraTypeChangedPayload
{
	GENERATED_BODY()

	UPROPERTY()
	ECameraType NewCameraType = ECameraType::None;
	UPROPERTY()
	ECameraType OldCameraType = ECameraType::None;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCameraTypeChanged, const FCameraTypeChangedPayload&, CameraTypeChangedPayload);

///////////////////////////
// Mouse/Gamepad Input Changed
///////////////////////////
USTRUCT(BlueprintType)
struct FGamepadInputChangedPayload
{
	GENERATED_BODY()
	FGamepadInputChangedPayload(){ bIsNowUsingGamepad = true; }
	FGamepadInputChangedPayload(const bool bInIsNowUsingGamepad) : bIsNowUsingGamepad(bInIsNowUsingGamepad)	{ }

	UPROPERTY(BlueprintReadOnly)
	bool bIsNowUsingGamepad;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamepadInputChanged, const FGamepadInputChangedPayload&, GamepadInputChangedPayload);

///////////////////////////
// Ability equipped
///////////////////////////
USTRUCT(BlueprintType)
struct FNewAbilityEquippedPayload
{
	GENERATED_BODY()
	FNewAbilityEquippedPayload(){}
	FNewAbilityEquippedPayload(const TWeakInterfacePtr<IAbility> InEquippedAbility) : NewEquippedAbility(InEquippedAbility)	{ }
	
	TWeakInterfacePtr<IAbility> NewEquippedAbility;
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
	FNewAbilityAddedPayload(const TWeakInterfacePtr<IAbility> InAddedAbility) : AddedAbility(InAddedAbility)	{ }

	TWeakInterfacePtr<IAbility> AddedAbility;
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
	FAbilityRemovedPayload(const TWeakInterfacePtr<IAbility> InAbility) : RemovedAbility(InAbility) { }

	TWeakInterfacePtr<IAbility> RemovedAbility;
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
	TWeakObjectPtr<UWorld> NewStreamedWorld;
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
		SetActivationCount(1);
	}

	FTriggerEventPayload(bool bRunActivation) : bStartActivationImmediately(bRunActivation)
	{
		SetActivationCount(0);
	}

	FTriggerEventPayload(bool bRunActivation, int32 ActivationLevel) : bStartActivationImmediately(bRunActivation)
	{
		SetActivationCount(ActivationLevel);
	}
	
	FTriggerEventPayload(bool bRunActivation, const FGameplayTag& ActivationTag, int32 ActivationLevel) : bStartActivationImmediately(bRunActivation)
	{
		AddActivationTagLevel(ActivationTag, ActivationLevel);
	}

	FTriggerEventPayload(bool bRunActivation, const TMap<FGameplayTag, int32>& ActivationTagLevels) : bStartActivationImmediately(bRunActivation), ActivationTagLevels(ActivationTagLevels) { }
	
	void SetActivationCount(int32 ActivationLevel)
	{
		AddActivationTagLevel(CommonGameTriggerEvent::TriggerActivationCount, ActivationLevel);
	}

	void AddActivationTagLevel(const FGameplayTag& ActivationTag, int32 ActivationLevel)
	{
		ActivationTagLevels.Add(ActivationTag, ActivationLevel);
	}

	// Should Activation run immediately after this trigger event
	// Activation can happen after the trigger is released instead of trigger pulled (e.g. Bow), so it should be false
	// For other things (like hitscan guns and buffs/AoE abilities), should be true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStartActivationImmediately = true;
	// If a Montage handles activation instead of the Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMontageDrivesActivation = false;

	// Optional: The tags and associated levels for activation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> ActivationTagLevels;
	// Optional: Location info to provide Activation mechanism (useful for things like throwing grenades where targeting happens during Trigger) 
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
	TWeakObjectPtr<AActor> DyingActor = nullptr;
	UPROPERTY()
	TWeakObjectPtr<AActor> KillingActor = nullptr;
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
	UPROPERTY(BlueprintReadOnly)
	int32 MaxWounds = 1;
	UPROPERTY(BlueprintReadOnly)
	float Delta = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float Percentage = 0.f;
	UPROPERTY(BlueprintReadOnly)
	bool bWasDamage = false;
	UPROPERTY(BlueprintReadOnly)
	bool bNaturalChange = false;
	UPROPERTY(BlueprintReadOnly)
	FDamageHitReactEvent DamageHitReactEvent = FDamageHitReactEvent();
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> ReceivingActor = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> InstigatingActor = nullptr;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWoundHealthChanged, const FCurrentWoundEventPayload&, CurrentWoundEventPayload);

///////////////////////////////////////
// MANA UPDATE
///////////////////////////////////////

USTRUCT(BlueprintType)
struct FEnergyChangedEventPayload
{
	GENERATED_BODY()

	FEnergyChangedEventPayload() { }	
	FEnergyChangedEventPayload(const float InCurrentEnergy, const float InMaxEnergy)
	{
		CurrentEnergy = InCurrentEnergy;
		MaxEnergy = InMaxEnergy;
	}

	UPROPERTY(BlueprintReadOnly)
	float CurrentEnergy = -1.f;
	UPROPERTY(BlueprintReadOnly)
	float MaxEnergy = -1.f;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnergyAmountChangedEvent, const FEnergyChangedEventPayload&, EnergyChangedEventPayload);

///////////////////////////
// AMMO AMOUNT CHANGED
///////////////////////////
USTRUCT(BlueprintType)
struct FAmmoAmountChangedPayload
{
	GENERATED_BODY()

	FAmmoAmountChangedPayload() {}
	FAmmoAmountChangedPayload(int32 InCurrAmmoInClip, int32 InClipCapacity, int32 InCurrentAmmo, int32 InMaxClips)
		: CurrentAmmoInClip(InCurrAmmoInClip), ClipCapacity(InClipCapacity),  CurrentAmmo(InCurrentAmmo), MaxAmmo(InMaxClips) {}
	
	UPROPERTY()
	int32 CurrentAmmoInClip = 0;
	UPROPERTY()
	int32 ClipCapacity = 0;
	UPROPERTY()
	int32 CurrentAmmo = 0;
	UPROPERTY()
	int32 MaxAmmo = 0;
	
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
	TWeakObjectPtr<AActor> InstigatingActor = nullptr;
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
	TWeakObjectPtr<AActor> InstigatingActor = nullptr;

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