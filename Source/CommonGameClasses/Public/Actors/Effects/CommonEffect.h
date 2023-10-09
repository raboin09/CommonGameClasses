// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionTree.h"
#include "API/Effect.h"
#include "NiagaraSystem.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "CommonEffect.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class COMMONGAMECLASSES_API UEffectData : public UObject
{
	GENERATED_BODY()
	
public:
	UEffectData()
	{
		// ImpactVFXRowHandle.DataTable = LoadObject<UDataTable>(nullptr, UTF8_TO_TCHAR("DataTable'/Game/_CUSTOM/Data/CUSTOM_ImpactVFX.CUSTOM_ImpactVFX'"));
		// ImpactSFXRowHandle.DataTable = LoadObject<UDataTable>(nullptr, UTF8_TO_TCHAR("DataTable'/Game/_CUSTOM/Data/CUSTOM_ImpactSFX.CUSTOM_ImpactSFX'"));
	}
	
	UPROPERTY(EditDefaultsOnly, Category="Initialization Data")
	FEffectInitializationData InitializationData;
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Conditions")
	UConditionTreeNode* Conditions;
	UPROPERTY(EditDefaultsOnly, Category="FX")
	bool bAttachVFXToActor = false;
	UPROPERTY(EditDefaultsOnly, Category="FX")
	FDataTableRowHandle ImpactVFXRowHandle;
	UPROPERTY(EditDefaultsOnly, Category="FX")
	FDataTableRowHandle ImpactSFXRowHandle;
	UPROPERTY(EditDefaultsOnly, Category="Modifiers")
	TMap<FGameplayTag, FModifierExpression> EffectModifiers;
};

class USoundCue;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonEffect : public AActor, public IEffect
{
	GENERATED_BODY()
	
public:	
	ACommonEffect();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//////////////////////////
	/// IEffect overrides
	//////////////////////////
	FORCEINLINE virtual const FEffectInitializationData& GetEffectInitializationData() override { return EffectData->InitializationData; };
	FORCEINLINE virtual const TArray<FGameplayTag>& GetEffectTags() const override { return EffectData->InitializationData.EffectTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetRemoveEffectTags() const override { return EffectData->InitializationData.RemoveEffectsWithTags; }
	virtual void SetEffectContext(const FEffectContext& InContext) override;
	virtual void PlayEffectFX() override;
	virtual bool TryActivateEffect() override;
	virtual void DestroyEffect() override;

protected:

	virtual bool CanActivateEffect();
	
	//////////////////////////
	/// BaseEffect code
	//////////////////////////
	
	// Must override in child classes
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void K2_OnActivateEffect();
	// Optionally override one of these in child classes
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void K2_OnDestroyEffect();
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON")
	UFXSystemAsset* K2_GetEffectParticleSystem();
	virtual UFXSystemAsset* K2_GetEffectParticleSystem_Implementation();
	
	// Optionally override one of these in child classes
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON")
	USoundCue* K2_GetEffectSound();
	virtual USoundCue* K2_GetEffectSound_Implementation();
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "CUSTOM")
	UEffectData* EffectData;
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
	// If Effect added tags and EffectData has bShouldRemoveAppliedTagsWhenDestroyed, remove applied tags
	void Internal_AddAndRemoveTagsFromReceiver_Deactivation();
	
	FORCEINLINE virtual const TArray<FGameplayTag>& GetBlockedTags() const { return EffectData->InitializationData.ValidTargets.BlockedTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetRequiredTags() const { return EffectData->InitializationData.ValidTargets.RequiredTags; }
};
