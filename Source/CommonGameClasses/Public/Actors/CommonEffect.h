// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Effect.h"
#include "NiagaraSystem.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "CommonEffect.generated.h"

class UNiagaraSystem;
class USoundCue;

UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class COMMONGAMECLASSES_API UBaseEffectData : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseEffectData()
	{
		// ImpactVFXRowHandle.DataTable = LoadObject<UDataTable>(nullptr, UTF8_TO_TCHAR("DataTable'/Game/_CUSTOM/Data/CUSTOM_ImpactVFX.CUSTOM_ImpactVFX'"));
		// ImpactSFXRowHandle.DataTable = LoadObject<UDataTable>(nullptr, UTF8_TO_TCHAR("DataTable'/Game/_CUSTOM/Data/CUSTOM_ImpactSFX.CUSTOM_ImpactSFX'"));
	}
	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FEffectInitializationData EffectData;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bAttachVFXToActor = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FDataTableRowHandle ImpactVFXRowHandle;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FDataTableRowHandle ImpactSFXRowHandle;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Modifiers")
	TMap<FGameplayTag, FModifierExpression> EffectModifiers;
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

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonEffect : public AActor, public IEffect
{
	GENERATED_BODY()
	
public:	
	ACommonEffect();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//////////////////////////
	/// IEffect overrides
	//////////////////////////
	FORCEINLINE virtual void SetEffectContext(const FEffectContext& InContext) override { EffectContext = InContext; };
	FORCEINLINE virtual const FEffectInitializationData& GetEffectInitializationData() override { return EffectDataObj->EffectData; };
	FORCEINLINE virtual const TArray<FGameplayTag>& GetBlockedTags() const override { return EffectDataObj->EffectData.ValidTargets.BlockedTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetEffectTags() const override { return EffectDataObj->EffectData.EffectTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetRequiredTags() const override { return EffectDataObj->EffectData.ValidTargets.RequiredTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetRemoveEffectTags() const override { return EffectDataObj->EffectData.RemoveEffectsWithTags; }
	virtual void PlayEffectFX() override;
	virtual void ActivateEffect() override;
	virtual void DestroyEffect() override;
	
protected:

	//////////////////////////
	/// BaseEffect code
	//////////////////////////
	
	// Must override in child classes
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON")
	void K2_ActivateEffect();
	virtual void K2_ActivateEffect_Implementation();

	// Optionally override one of these in child classes
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON")
	void K2_OnDestroyEffect();
	virtual void K2_OnDestroyEffect_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON")
	class UFXSystemAsset* K2_GetEffectParticleSystem();
	virtual UFXSystemAsset* K2_GetEffectParticleSystem_Implementation();
	
	// Optionally override one of these in child classes
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON")
	USoundCue* K2_GetEffectSound();
	virtual USoundCue* K2_GetEffectSound_Implementation();
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "CUSTOM")
	UBaseEffectData* EffectDataObj;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM")
	FEffectContext EffectContext;

	UPROPERTY(Transient)
	UFXSystemComponent* EffectVFX;
private:
	void Internal_PlayEffectSound();
	void Internal_PlayEffectParticleSystem();
	bool Internal_IsValidHeadshot() const;

	// Add and remove tags
	void Internal_AddAndRemoveTagsFromReceiver_Activation();
	// If Effect added tags and EffectDataObj has bShouldRemoveAppliedTagsWhenDestroyed, remove applied tags
	void Internal_AddAndRemoveTagsFromReceiver_Deactivation();	
};
