// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CombatTypes.generated.h"

class UNiagaraSystem;
class USoundCue;

UENUM(BlueprintType)
enum class EAffectedAffiliation : uint8
{
	Allies,
	Enemies,
	Neutral,
	All
};

USTRUCT(BlueprintType)
struct FEffectImpactVFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* DefaultFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* ConcreteFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* DirtFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* WaterFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* MetalFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* WoodFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* GlassFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* GrassFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* FleshFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* SandFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* PlasticFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* IceFX = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	UNiagaraSystem* FleshHeadshotFX = nullptr;
};

USTRUCT(BlueprintType)
struct FEffectImpactSFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* DefaultSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* ConcreteSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* DirtSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* WaterSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* MetalSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* WoodSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* GlassSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* GrassSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* FleshSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* SandSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* PlasticSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* IceSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	USoundCue* FleshHeadshotSound = nullptr;
};