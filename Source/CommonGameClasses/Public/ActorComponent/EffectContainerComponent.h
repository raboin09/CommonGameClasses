// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Effect.h"
#include "Components/ActorComponent.h"
#include "Types/AbilityTypes.h"
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
	}

	FTickingEffect(TScriptInterface<IEffect> IncomingEffect, int32 InTickModulus, int32 InExpirationTime, int32 InTickID)
	{
		TickModulus = InTickModulus;
		RemainingTickActivations = InExpirationTime;
		TickingEffect = IncomingEffect;
		TickID = InTickID;
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
	
	void TryApplyEffectToContainerFromHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);
	void TryApplyEffectToContainer(TSubclassOf<AActor> BaseEffectClass, AActor* InstigatingActor);
	
	// This is static so that projectile/hitscan misses can still create the Sound/Visual FX on the ground 
	static TScriptInterface<IEffect> CreateEffectInstanceFromHitResult(UObject* ContextObject, TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult = true);
	
protected:
	virtual void BeginPlay() override;
	
private:
	TScriptInterface<IEffect> CreateEffectInstance(TSubclassOf<AActor> BaseEffectClass, AActor* InstigatingActor) const;

	void Internal_TryActivateEffect(TScriptInterface<IEffect> IncomingEffect);
	void Internal_ApplyEffect(TScriptInterface<IEffect> IncomingEffect);
	void Internal_RemoveEffectsWithTags(const TArray<FGameplayTag>& InTags, TScriptInterface<IEffect> IncomingEffect);
	
	void Internal_TickEffects();
	void Internal_TickEffect(int32 CurrentTickingEffectKey);
	void Internal_TryStartTicking();
	void Internal_StopTicking();
	
	bool CanApplyEffect(TScriptInterface<IEffect> IncomingEffect) const;
	int32 GetTickingEffectIndex(const UClass* EffectClass);
	bool HasEffectClassAlready(const UClass* EffectClass) const;
	static int32 GenerateModulus(EEffectInterval EffectInterval);
	static double ConvertIntervalToNumTicks(EEffectInterval EffectInterval);
	static int32 GenerateNumTicks(EEffectInterval EffectInterval, double Duration);
	
	FORCEINLINE TArray<int32> GetKeys() const { TArray<int32> Keys; EffectsToTick.GetKeys(Keys); return Keys; }
	void Internal_AddEffectToTickContainer(TScriptInterface<IEffect> IncomingEffect);
	static void Internal_ActivateEffect(const FTickingEffect& IncomingEffect);
	void Internal_DestroyEffect(TScriptInterface<IEffect> IncomingEffect, int32 TickID);
	FTickingEffect Internal_GenerateTickingEffectStruct(TScriptInterface<IEffect> EffectInitializationData);

	UPROPERTY()
	const UWorld* CachedWorld;
	TSet<UClass*> CurrentEffectClasses;
	TMap<int32, FTickingEffect> EffectsToTick;
	
	FTimerHandle Timer_EffectTicker;
	int32 TickCounter = 0;
	int32 TickIDCounter = 0;
	bool bIsTicking;
};
