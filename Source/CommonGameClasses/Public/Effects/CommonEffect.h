// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionTree.h"
#include "API/Effect.h"
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
		Conditions = CreateDefaultSubobject<UConditionTreeNode>(TEXT("ConditionTree"));
	}

	UPROPERTY(EditDefaultsOnly, Category="Initialization Data", meta=(ShowOnlyInnerProperties))
	FEffectInitializationData InitializationData;
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Conditions")
	TObjectPtr<UConditionTreeNode> Conditions;
	UPROPERTY(EditDefaultsOnly, Category="FX")
	bool bAttachVFXToActor = false;
	UPROPERTY(EditDefaultsOnly, Category="Modifiers")
	TMap<FGameplayTag, FModifierExpression> EffectModifiers;
};

class USoundCue;

UCLASS(Abstract, Blueprintable, HideCategories=("ActorTick", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "Physics", "WorldPartition", "Events",
		"LevelInstance", "Cooking", "DataLayers", "Level Instance", "World Partition", "Actor Tick"))
class COMMONGAMECLASSES_API ACommonEffect : public AActor, public IEffect
{
	GENERATED_BODY()
	
public:	
	ACommonEffect();
	
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
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	bool BPI_CanActivateEffect();
	
	//////////////////////////
	/// BaseEffect code
	//////////////////////////
	
	// BP event that fires after effect has successfully been activated
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void BPI_ActivateEffect();
	// Optionally override one of these in child classes
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void BPI_DestroyEffect();

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void BPI_PlayEffectSound(const FEffectContext& Context);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void BPI_PlayEffectVFX(const bool bAttachVFXToActor, const FEffectContext& Context);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "CUSTOM")
	TObjectPtr<UEffectData> EffectData;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM")
	FEffectContext EffectContext;

private:
	
	// Add and remove tags
	void Internal_AddAndRemoveTagsFromReceiver_Activation();
	// If Effect added tags and EffectData has bShouldRemoveAppliedTagsWhenDestroyed, remove applied tags
	void Internal_AddAndRemoveTagsFromReceiver_Deactivation();
	
	FORCEINLINE virtual const TArray<FGameplayTag>& GetBlockedTags() const { return EffectData->InitializationData.ValidTargets.BlockedTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetRequiredTags() const { return EffectData->InitializationData.ValidTargets.RequiredTags; }
};
