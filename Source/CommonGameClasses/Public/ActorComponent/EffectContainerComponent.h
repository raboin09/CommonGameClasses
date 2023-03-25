// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Effect.h"
#include "Components/ActorComponent.h"
#include "EffectContainerComponent.generated.h"

USTRUCT()
struct FTickingEffect
{
	GENERATED_BODY()

	FTickingEffect()
	{
		TickingEffect = nullptr;
		TickModulus = -1;
		ExpirationTime = -1;
		TickID = nullptr;
	}

	FTickingEffect(TScriptInterface<IEffect> IncomingEffect, int32 InTickModulus, int32 InExpirationTime, UClass* InTickID)
	{
		TickModulus = InTickModulus;
		ExpirationTime = InExpirationTime;
		TickingEffect = IncomingEffect;
		TickID = InTickID;
	}

	TScriptInterface<IEffect> TickingEffect;
	int32 TickModulus;
	float ExpirationTime;
	UClass* TickID;
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
	void Internal_TickEffect(const UClass* CurrentTickingEffectKey);
	void Internal_StartTicking();
	void Internal_StopTicking();
	
	bool CanApplyEffect(TScriptInterface<IEffect> IncomingEffect) const;
	static int32 ConvertInterval(EEffectInterval EffectInterval);
	
	void Internal_AddEffectToTickContainer(TScriptInterface<IEffect> IncomingEffect);
	static void Internal_ActivateEffect(TScriptInterface<IEffect> IncomingEffect);
	void Internal_DestroyEffect(TScriptInterface<IEffect> IncomingEffect, const UClass* TickID);

	UPROPERTY()
	const UWorld* CachedWorld;
	
	TMap<UClass*, FTickingEffect> EffectsToTick;
	FTimerHandle Timer_EffectTicker;
	
	const float QuarterSecondTick = .25f;
	const float HalfSecondTick = .5f;
	const float FullSecondTick = 1.f;

	const float MaxCycles = 5;
	const float TotalTicksPerCycle = 4 * MaxCycles;
	
	int32 TickCounter = 1;
	bool bIsTicking;
};