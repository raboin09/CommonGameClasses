// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonCombatTypes.generated.h"

class UNiagaraSystem;
class USoundCue;

UENUM(BlueprintType)
enum class EHitReactType : uint8
{
	None,
	Knockback_Tiny,
	Knockback_VeryLight,
	Knockback_Light,
	Knockback_Medium,
	Knockback_Heavy,
	Knockback_VeryHeavy,
	Knockback_Huge,
	HitReact_Light,
	HitReact_Chainsaw
};

USTRUCT(BlueprintType)
struct FDamageHitReactEvent
{
	GENERATED_BODY()
	
	FVector HitDirection;
	float DamageTaken;
	FHitResult HitResult;
	EHitReactType HitReactType = EHitReactType::None;
	EHitReactType DeathReactType = EHitReactType::None;
};


UENUM(BlueprintType)
enum class EEffectStatType
{
	None,
	MaxWounds UMETA(DisplayName = "Max Wounds (Resource Pools)"),
	Damage_All UMETA(DisplayName = "Damage All (Affects Health and Shields)"),
	Damage_Health UMETA(DisplayName = "Damage Health (ONLY Affects Health)"),
	Damage_Shield UMETA(DisplayName = "Damage Shields (ONLY Affects Shields)"),
	HealHealth UMETA(DisplayName = "Heal Health (ONLY Affects Health)"),
	HealShield UMETA(DisplayName = "Heal Shields (ONLY Affects Shields)"),
	MoveSpeed UMETA(DisplayName = "Move Speed (Affects Health and Shields)"),
};

USTRUCT(BlueprintType)
struct FEffectImpactVFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> DefaultFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> ConcreteFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> DirtFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> WaterFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> MetalFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> WoodFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> GlassFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> GrassFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> FleshFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> SandFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> PlasticFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> IceFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<UNiagaraSystem> FleshHeadshotFX = nullptr;
};

USTRUCT(BlueprintType)
struct FEffectImpactSFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> DefaultSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> ConcreteSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> DirtSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> WaterSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> MetalSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> WoodSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> GlassSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> GrassSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> FleshSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> SandSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> PlasticSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> IceSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSoftObjectPtr<USoundCue> FleshHeadshotSound = nullptr;
};