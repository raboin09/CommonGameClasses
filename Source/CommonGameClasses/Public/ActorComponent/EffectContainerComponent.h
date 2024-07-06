// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Effect.h"
#include "Components/ActorComponent.h"
#include "Types/CommonAbilityTypes.h"
#include "EffectContainerComponent.generated.h"

USTRUCT()
struct FTickingEffect
{
	GENERATED_BODY()

	FTickingEffect()
	{
		TickingEffect = nullptr;
		TickModulus = -1;
		RemainingTickActivations = -1;
		TickID = -1;
		ExpirationTime = -1.f;
	}

	FTickingEffect(TScriptInterface<IEffect> IncomingEffect, int32 InTickModulus, int32 InExpirationTime, int32 InTickID)
	{
		TickModulus = InTickModulus;
		RemainingTickActivations = InExpirationTime;
		TickingEffect = IncomingEffect;
		TickID = InTickID;
		ExpirationTime = -1.f;
	}

	bool operator==(const FTickingEffect& OtherEffect) const
	{
		return OtherEffect.TickingEffect == TickingEffect;
	}

	TScriptInterface<IEffect> TickingEffect;
	int32 TickModulus;
	int32 TickID;
	// Used for periodic activations (Once every .25/.5/1/2/5 seconds)
	int32 RemainingTickActivations;
	// Used for ApplyOnce
	double ExpirationTime;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMMONGAMECLASSES_API UEffectContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEffectContainerComponent();

	void TryRemoveAllTaggedEffects(const FGameplayTag& RemoveEffectsWithTag);
	void TryRemoveAllEffectsOfClass(TSubclassOf<AActor> EffectClassToRemove);
	
	void TryApplyEffectToContainerFromHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, TWeakObjectPtr<AActor> InstigatingActor, bool bShouldRotateHitResult = true);
	void TryApplyEffectToContainer(TSubclassOf<AActor> BaseEffectClass, TWeakObjectPtr<AActor> InstigatingActor);
	
	// This is static so that projectile/hitscan misses can still create the Sound/Visual FX on the ground 
	static TScriptInterface<IEffect> CreateEffectInstanceFromHitResult(TWeakObjectPtr<UObject> ContextObject, TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, TWeakObjectPtr<AActor> InstigatingActor, bool bShouldRotateHitResult = true);
	
protected:
	virtual void BeginPlay() override;
	
private:
	TScriptInterface<IEffect> CreateEffectInstance(TSubclassOf<AActor> BaseEffectClass, TWeakObjectPtr<AActor> InstigatingActor) const;

	bool Internal_TryActivateEffect(TScriptInterface<IEffect> IncomingEffect);
	void Internal_ApplyEffect(TScriptInterface<IEffect> IncomingEffect);
	void Internal_RemoveEffectsWithTags(const TArray<FGameplayTag>& InTags, TScriptInterface<IEffect> IncomingEffect);
	void Internal_RemoveEffectsWithTag(const FGameplayTag& InTag, TScriptInterface<IEffect> IncomingEffect);
	void Internal_RemoveEffectsWithClass(TSubclassOf<AActor> EffectClassToRemove);
	
	void Internal_TickEffects();
	void Internal_TickEffect(int32 CurrentTickingEffectKey);
	void Internal_TryStartTicking();
	void Internal_StopTicking();
	
	int32 GetTickingEffectIndex(const UClass* EffectClass);
	bool HasEffectClassAlready(const UClass* EffectClass) const;
	
	FORCEINLINE TArray<int32> GetKeys() const { TArray<int32> Keys; EffectsToTick.GetKeys(Keys); return Keys; }
	void Internal_AddEffectToTickContainer(TScriptInterface<IEffect> IncomingEffect);
	static void Internal_ActivateEffect(const FTickingEffect& IncomingEffect);
	void Internal_DestroyEffect(TScriptInterface<IEffect> IncomingEffect, int32 TickID);
	FTickingEffect Internal_GenerateTickingEffectStruct(TScriptInterface<IEffect> EffectInitializationData);

	UPROPERTY()
	TWeakObjectPtr<UWorld> CachedWorld;
	TSet<UClass*> CurrentEffectClasses;
	TMap<int32, FTickingEffect> EffectsToTick;
	
	FTimerHandle Timer_EffectTicker;
	int32 TickCounter = 0;
	int32 TickIDCounter = 0;
	bool bIsTicking;
};
