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

	UPROPERTY(EditDefaultsOnly, meta=(ShowOnlyInnerProperties))
	FEffectInitializationData InitializationData;
	UPROPERTY(EditDefaultsOnly, Instanced, meta=(ShowInnerProperties, FullyExpand="true"))
	TObjectPtr<UConditionTreeNode> Conditions;
	UPROPERTY(EditDefaultsOnly, meta=(ForceInlineRow))
	TMap<FGameplayTag, FModifierExpression> EffectModifiers;
};

class USoundCue;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("COMMON"), PrioritizeCategories = ("COMMON"), HideCategories=(COMMON_IGNORE_CATEGORIES))
class COMMONGAMECLASSES_API ACommonEffect : public AActor, public IEffect
{
	GENERATED_BODY()
	
public:	
	ACommonEffect();

	//~ Begin IEffect Interface
	FORCEINLINE virtual const FEffectInitializationData& GetEffectInitializationData() override { return EffectData->InitializationData; };
	FORCEINLINE virtual const TArray<FGameplayTag>& GetEffectTags() const override { return EffectData->InitializationData.EffectTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetRemoveEffectTags() const override { return EffectData->InitializationData.RemoveEffectsWithTags; }
	virtual void SetEffectContext(const FEffectContext& InContext) override;
	virtual void PlayEffectFX() override;
	virtual bool TryActivateEffect() override;
	virtual void DestroyEffect() override;
	//~ Begin IEffect Interface
	
protected:
	virtual bool CanActivateEffect();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Effect")
	bool BPI_CanActivateEffect();
	
	//////////////////////////
	/// BaseEffect code
	//////////////////////////
	
	// BP event that fires after effect has successfully been activated
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Effect")
	void BPI_ActivateEffect();
	// Optionally override one of these in child classes
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Effect")
	void BPI_DestroyEffect();

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Effect")
	void BPI_PlayEffectSound(const FEffectContext& Context);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Effect")
	void BPI_PlayEffectVFX(const FEffectContext& Context);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "COMMON|Effect", meta=(ShowInnerProperties, FullyExpand="true"))
	TObjectPtr<UEffectData> EffectData;
	UPROPERTY(BlueprintReadOnly, Category = "COMMON|Effect")
	FEffectContext EffectContext;

private:
	
	// Add and remove tags
	void Internal_AddAndRemoveTagsFromReceiver_Activation();
	// If Effect added tags and EffectData has bShouldRemoveAppliedTagsWhenDestroyed, remove applied tags
	void Internal_AddAndRemoveTagsFromReceiver_Deactivation();
	
	FORCEINLINE virtual const TArray<FGameplayTag>& GetBlockedTags() const { return EffectData->InitializationData.ValidTargets.BlockedTags; }
	FORCEINLINE virtual const TArray<FGameplayTag>& GetRequiredTags() const { return EffectData->InitializationData.ValidTargets.RequiredTags; }
};
