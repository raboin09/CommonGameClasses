// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonCombatTypes.generated.h"

class UNiagaraSystem;
class USoundCue;

/**
 * Defines various types of hit reactions and knockback effects in response to impacts.
 *
 * This enumeration is used to classify the intensity and type of reactions triggered by
 * an entity when it is hit or subjected to a force. It enables the implementation of
 * different responses, such as knockback or specific hit animations, based on the
 * gameplay context.
 *
 * Available reaction types:
 * - None: No reaction is applied.
 * - Knockback_Tiny, Knockback_VeryLight, Knockback_Light, Knockback_Medium, etc.:
 *   Represent varying levels of knockback intensity, ranging from slight displacements
 *   to severe impacts.
 * - HitReact_Light: Specifies a light hit reaction usually tied to animations or minor effects.
 * - HitReact_Special1: Denotes a custom or specialized hit reaction distinct from standard reactions.
 */
UENUM(BlueprintType)
enum class EHitReactType : uint8
{
	None,
	Ragdoll_Tiny,
	Ragdoll_VeryLight,
	Ragdoll_Light,
	Ragdoll_Medium,
	Ragdoll_Heavy,
	Ragdoll_VeryHeavy,
	Ragdoll_Huge,
	Launch_Tiny,
	Launch_VeryLight,
	Launch_Light,
	Launch_Medium,
	Launch_Heavy,
	Launch_VeryHeavy,
	Launch_Huge,
};

/**
 * Represents an event triggered in response to damage being received with specific hit reaction logic.
 *
 * This class is designed to encapsulate details about an incoming damage event and the corresponding
 * reaction that should be executed. It includes necessary information to process reactions, such as
 * the type, magnitude, or direction of the damage. Typically, it is part of systems handling combat,
 * character response to impacts, or environmental interactions causing harm.
 */
USTRUCT(BlueprintType)
struct FDamageHitReactEvent
{
	GENERATED_BODY()

	/**
	 * A vector representing the direction of an incoming hit or impact.
	 *
	 * This variable is used to determine the directional flow of an external force, such as in hit reaction logic.
	 * It is typically normalized to ensure consistency in calculations and can be derived from sources such as
	 * the relative position of the attacker or the trajectory of a projectile.
	 */
	FVector HitDirection;
	/**
	 * Represents the amount of damage taken by a character or entity.
	 *
	 * This variable captures the value of damage inflicted, either directly applied
	 * or calculated from an external source such as a hit event or damage modifier.
	 * It is part of the combat system and is used in tandem with other damage-related
	 * mechanics, such as resource depletion, hit reactions, or event broadcasting.
	 */
	float DamageTaken;
	/**
	 * Represents the result of a single physics or collision hit in the system.
	 *
	 * This variable encapsulates detailed information about a hit event,
	 * such as the impact point, surface normal, and the body hit during
	 * gameplay interactions. It is primarily used in combat mechanics, damage
	 * reactions, and character animations for processing events like knockback,
	 * hit reaction, and death sequences.
	 */
	FHitResult HitResult;
	/**
	 * @brief Specifies the type of hit reaction assigned to an entity.
	 *
	 * Used to indicate various impact responses such as knockback or hit reactions.
	 * - EHitReactType::None: No reaction is triggered.
	 * - EHitReactType::Knockback (Tiny, Light, Medium, etc.): Specifies degrees of knockback responses.
	 * - EHitReactType::HitReact_Light and others: Encodes standard or specialized hit reactions.
	 */
	EHitReactType HitReactType = EHitReactType::None;
	/**
	 * Specifies the reaction type to be used when the character's death occurs.
	 *
	 * The value is of type EHitReactType and represents the intensity or type of
	 * knockback or reaction that will be applied to the character upon their death.
	 * Commonly used in scenarios involving combat or impact-based events where
	 * death animations or physics need to be triggered.
	 *
	 * Default Value:
	 * - EHitReactType::None (indicating no reaction should occur by default).
	 */
	EHitReactType DeathReactType = EHitReactType::None;
};


/**
 * Represents the types of statistical effects that can be applied to an entity.
 *
 * This enumeration specifies different categories of effects that modify key attributes
 * such as health, shields, movement speed, or other gameplay-related stats.
 * It serves as a foundation for implementing gameplay mechanics like damage, healing,
 * or alterations to movement dynamics.
 *
 * Available effect types:
 * - None: No effect is applied.
 * - Damage_All: Applies damage that affects both health and shields.
 * - Damage_Health: Applies damage that exclusively affects health.
 * - Damage_Shield: Applies damage that exclusively affects shields.
 * - HealHealth: Heals health exclusively.
 * - HealShield: Heals shields exclusively.
 * - MoveSpeed: Alters the movement speed of the affected entity.
 */
UENUM(BlueprintType)
enum class EEffectStatType : uint8
{
	None,
	Damage_All UMETA(DisplayName = "Damage All (Affects Health and Shields)"),
	Damage_Health UMETA(DisplayName = "Damage Health (ONLY Affects Health)"),
	Damage_Shield UMETA(DisplayName = "Damage Shields (ONLY Affects Shields)"),
	HealHealth UMETA(DisplayName = "Heal Health (ONLY Affects Health)"),
	HealShield UMETA(DisplayName = "Heal Shields (ONLY Affects Shields)"),
	MoveSpeed UMETA(DisplayName = "Move Speed"),
};

/**
 * Represents a collection of visual effects that are triggered upon impact with various surface types.
 *
 * This structure is utilized for defining particle effects or visual responses generated during interactions
 * such as collisions, combat, or environmental events. Each field corresponds to a specific material type
 * and is associated with an optional Niagara System asset to provide the appropriate effect for that surface.
 *
 * Fields:
 * - DefaultFX: The default visual effect applied when no specific surface type matches.
 * - ConcreteFX: Effect triggered when interacting with concrete surfaces.
 * - DirtFX: Effect triggered when interacting with dirt surfaces.
 * - WaterFX: Effect triggered when interacting with water surfaces.
 * - MetalFX: Effect triggered when interacting with metal surfaces.
 * - WoodFX: Effect triggered when interacting with wood surfaces.
 * - GlassFX: Effect triggered when interacting with glass surfaces.
 * - GrassFX: Effect triggered when interacting with grass surfaces.
 * - FleshFX: Effect triggered when interacting with generic flesh surfaces.
 * - SandFX: Effect triggered when interacting with sand surfaces.
 * - PlasticFX: Effect triggered when interacting with plastic surfaces.
 * - IceFX: Effect triggered when interacting with ice surfaces.
 * - FleshHeadshotFX: Special effect triggered when interacting with flesh during a headshot event.
 */
USTRUCT(BlueprintType)
struct FEffectImpactVFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> DefaultFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> ConcreteFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> DirtFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> WaterFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> MetalFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> WoodFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> GlassFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> GrassFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> FleshFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> SandFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> PlasticFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> IceFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<UNiagaraSystem> FleshHeadshotFX = nullptr;
};

USTRUCT(BlueprintType)
struct FEffectImpactSFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> DefaultSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> ConcreteSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> DirtSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> WaterSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> MetalSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> WoodSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> GlassSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> GrassSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> FleshSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> SandSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> PlasticSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> IceSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TSoftObjectPtr<USoundCue> FleshHeadshotSound = nullptr;
};