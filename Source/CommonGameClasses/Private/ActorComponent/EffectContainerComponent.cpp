 // Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Effects/CommonEffect.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/CommonCombatUtils.h"
#include "Utils/CommonWorldUtils.h"

DECLARE_CYCLE_STAT(TEXT("CommonClasses_EffectContainerTick"), STAT_CommonClasses_StatsEffectContainerTicks, STATGROUP_StatSystem);

UEffectContainerComponent::UEffectContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsTicking = false;
	CachedWorld = nullptr;
}

void UEffectContainerComponent::TryApplyEffectToContainerFromHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, TWeakObjectPtr<AActor> InstigatingActor, bool bShouldRotateHitResult)
{
	if(!BaseEffectClass || !BaseEffectClass->ImplementsInterface(UEffect::StaticClass()))
	{
		return;
	} 
	const TScriptInterface<IEffect> BaseEffect = CreateEffectInstanceFromHitResult(this, BaseEffectClass, Impact, InstigatingActor, bShouldRotateHitResult);
	Internal_ApplyEffect(BaseEffect);
}

void UEffectContainerComponent::TryApplyEffectToContainer(TSubclassOf<AActor> BaseEffectClass, TWeakObjectPtr<AActor> InstigatingActor)
{
	if(!BaseEffectClass || !BaseEffectClass->ImplementsInterface(UEffect::StaticClass()))
	{
		return;
	}
	const TScriptInterface<IEffect> BaseEffect = CreateEffectInstance(BaseEffectClass, InstigatingActor);
	Internal_ApplyEffect(BaseEffect);
}

TScriptInterface<IEffect> UEffectContainerComponent::CreateEffectInstanceFromHitResult(TWeakObjectPtr<UObject> ContextObject, TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, TWeakObjectPtr<AActor> InstigatingActor, bool bShouldRotateHitResult)
{
	if(!ContextObject.IsValid())
	{
		return nullptr;
	}

	if(!BaseEffectClass)
	{
		return nullptr;
	}
	
	FTransform SpawnTransform = FTransform(Impact.Location);
	if (bShouldRotateHitResult)
	{
		SpawnTransform = FTransform(UCommonCombatUtils::GetRotationFromComponentHit(Impact), Impact.ImpactPoint);
	}
	ACommonEffect* EffectActor = UCommonWorldUtils::SpawnActorToCurrentWorld_Deferred<ACommonEffect>(BaseEffectClass, InstigatingActor.Get(), Cast<APawn>(InstigatingActor));
	TScriptInterface<IEffect> SpawnedEffect;
	SpawnedEffect.SetObject(EffectActor);
	SpawnedEffect.SetInterface(Cast<IEffect>(EffectActor));
	FEffectContext EffectContext;
	EffectContext.InstigatingActor = InstigatingActor;
	EffectContext.ReceivingActor = Impact.GetActor();
	EffectContext.SurfaceHit = Impact;
	EffectContext.HitDirection = Impact.ImpactNormal;
	UCommonWorldUtils::FinishSpawningActor_Deferred(EffectActor, SpawnTransform);
	if (!SpawnedEffect)
	{
		return nullptr;
	}
	EffectActor->SetEffectContext(EffectContext);
	return SpawnedEffect;
}

void UEffectContainerComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedWorld = GetWorld();
}

TScriptInterface<IEffect> UEffectContainerComponent::CreateEffectInstance(TSubclassOf<AActor> BaseEffectClass, TWeakObjectPtr<AActor> InstigatingActor) const
{
	if (!GetOwner())
	{
		return nullptr;
	}

	if(!IsValid(BaseEffectClass))
	{
		return nullptr;
	}

	const FTransform& SpawnTransform = GetOwner()->GetActorTransform();

	ACommonEffect* EffectActor = UCommonWorldUtils::SpawnActorToCurrentWorld_Deferred<ACommonEffect>(BaseEffectClass.Get());
	if (!EffectActor)
	{
		return nullptr;
	}

	EffectActor->SetInstigator(Cast<APawn>(InstigatingActor));
	FEffectContext EffectContext;
	EffectContext.InstigatingActor = InstigatingActor;
	EffectContext.ReceivingActor = GetOwner();
	EffectActor->SetEffectContext(EffectContext);
	UCommonWorldUtils::FinishSpawningActor_Deferred(EffectActor, SpawnTransform);
	return EffectActor;
}

void UEffectContainerComponent::Internal_ApplyEffect(TScriptInterface<IEffect> IncomingEffect)
{
	if (!IncomingEffect)
	{
		return;
	}
	const FEffectInitializationData& EffectInitData = IncomingEffect->GetEffectInitializationData();

	if(EffectInitData.DurationType == EEffectDurationType::Instant)
	{
		Internal_TryActivateEffect(IncomingEffect);
		Internal_DestroyEffect(IncomingEffect, -1);
	} else
	{
		Internal_AddEffectToTickContainer(IncomingEffect);
	}
}

void UEffectContainerComponent::Internal_RemoveEffectsWithTags(const TArray<FGameplayTag>& InTags, TScriptInterface<IEffect> IncomingEffect)
{
	for(const FGameplayTag& CurrTag : InTags)
	{
		TArray<int32> EffectKeys;
		EffectsToTick.GetKeys(EffectKeys);
		for(const int32 Key : EffectKeys)
		{
			if(const auto Effect = EffectsToTick.Find(Key); Effect->TickingEffect)
			{
				if(Effect->TickingEffect->GetEffectTags().Contains(CurrTag) && Effect->TickingEffect != IncomingEffect)
				{
					Effect->TickingEffect->DestroyEffect();
					EffectsToTick.Remove(Key);
				}
			}
		}
	}
}

bool UEffectContainerComponent::Internal_TryActivateEffect(TScriptInterface<IEffect> IncomingEffect)
{
	if(!IncomingEffect)
	{
		return false;
	}

	if(IncomingEffect->TryActivateEffect())
	{
		Internal_RemoveEffectsWithTags(IncomingEffect->GetRemoveEffectTags(), IncomingEffect);
		return true;
	}
	return false;
}

void UEffectContainerComponent::Internal_TickEffects()
{
	SCOPE_CYCLE_COUNTER(STAT_CommonClasses_StatsEffectContainerTicks);
	if (++TickCounter > EFFECT_TICKS_PER_CYCLE)
	{
		TickCounter = 0;
	}
	for (const int32 CurrentTickingEffectKey : GetKeys())
	{
		Internal_TickEffect(CurrentTickingEffectKey);
	}
}

void UEffectContainerComponent::Internal_TickEffect(int32 CurrentTickingEffectKey)
{
	if(CachedWorld.IsStale() || !EffectsToTick.Contains(CurrentTickingEffectKey))
		return;

	FTickingEffect& TickingEffect = EffectsToTick[CurrentTickingEffectKey];
	const TScriptInterface<IEffect> CurrentEffect = TickingEffect.TickingEffect;
	if(!CurrentEffect)
		return;

	const FEffectInitializationData& EffectInitializationData = CurrentEffect->GetEffectInitializationData();

	// No need to run an Apply_Once effect
	// Check if ApplyOnce effect has expired
	if(EffectInitializationData.TickInterval == EEffectTickInterval::Apply_Once)
	{
		if(TickingEffect.ExpirationTime <= CachedWorld->GetTimeSeconds())
		{
			Internal_DestroyEffect(CurrentEffect, CurrentTickingEffectKey);
		}
		// Return no matter what if it's ApplyOnce
		return;
	}		

	// Check if it's not time to tick recurring effect yet
	if(TickCounter % TickingEffect.TickModulus != 0)
	{
		return;
	}
	
	const bool bIsInfinite = EffectInitializationData.DurationType == EEffectDurationType::Infinite;
	if (!bIsInfinite && --TickingEffect.RemainingTickActivations < 0)
	{
		Internal_DestroyEffect(CurrentEffect, CurrentTickingEffectKey);
		return;
	}
	
	const bool bWasSuccessfulActivation = Internal_TryActivateEffect(CurrentEffect);
	if(bWasSuccessfulActivation && EffectInitializationData.DurationType == EEffectDurationType::FirstActivation)
	{
		Internal_DestroyEffect(CurrentEffect, CurrentTickingEffectKey);
	}
}

void UEffectContainerComponent::Internal_TryStartTicking()
{
	if(bIsTicking || CachedWorld.IsStale())
	{
		return;
	}
	Internal_TickEffects();
	CachedWorld->GetTimerManager().SetTimer(Timer_EffectTicker, this, &ThisClass::Internal_TickEffects, EFFECT_TICK_RATE, true);
	bIsTicking = true;
}

void UEffectContainerComponent::Internal_StopTicking()
{
	if (CachedWorld.IsStale())
	{
		return;
	}
	CachedWorld->GetTimerManager().ClearTimer(Timer_EffectTicker);
	bIsTicking = false;
}

int32 UEffectContainerComponent::GetTickingEffectIndex(const UClass* EffectClass)
{
	TArray<int32> Keys;
	EffectsToTick.GetKeys(Keys);
	for(const int32 Key : Keys)
	{
		FTickingEffect TickingEffect = EffectsToTick[Key];
		if(TickingEffect.TickingEffect && TickingEffect.TickingEffect.GetObject()->GetClass() == EffectClass)
			return TickingEffect.TickID;
	}
	return -1;
}

bool UEffectContainerComponent::HasEffectClassAlready(const UClass* EffectClass) const
{
	TArray<int32> Keys;
	EffectsToTick.GetKeys(Keys);
	for(const int32 Key : Keys)
	{
		FTickingEffect TickingEffect = EffectsToTick[Key];
		if(TickingEffect.TickingEffect && TickingEffect.TickingEffect.GetObject()->GetClass() == EffectClass)
			return true;
	}
	return false;
}

FTickingEffect UEffectContainerComponent::Internal_GenerateTickingEffectStruct(TScriptInterface<IEffect> IncomingEffect)
{
	const FEffectInitializationData& EffectInitializationData = IncomingEffect->GetEffectInitializationData();
	FTickingEffect TickingEffect;
	TickingEffect.TickModulus = GenerateModulus(EffectInitializationData.TickInterval);
	TickingEffect.TickID = TickIDCounter++;
	TickingEffect.TickingEffect = IncomingEffect;
	const bool bIsInfiniteEffect = EffectInitializationData.DurationType == EEffectDurationType::Infinite;
	if(!bIsInfiniteEffect)
	{
		// Expiration is generally only used for ApplyOnce
		TickingEffect.ExpirationTime = CachedWorld->GetTimeSeconds() + EffectInitializationData.EffectDuration;
		// RemainingTickActivations is only used for periodic activations (Once every .25/.5/1/2/5 seconds)
		TickingEffect.RemainingTickActivations = GenerateNumTicks(EffectInitializationData.TickInterval, EffectInitializationData.EffectDuration);
	}
	return TickingEffect;
}

void UEffectContainerComponent::Internal_AddEffectToTickContainer(TScriptInterface<IEffect> IncomingEffect)
{
	if (!IncomingEffect || !CachedWorld.IsValid())
	{
		return;
	}
	UClass* IncomingClass = IncomingEffect.GetObject()->GetClass();
	const FTickingEffect NewTickingEffect = Internal_GenerateTickingEffectStruct(IncomingEffect);
	const bool bEffectCanStack = IncomingEffect->GetEffectInitializationData().bEffectCanStack;
	if(bEffectCanStack || !HasEffectClassAlready(IncomingClass))
	{
		// TickingEffects can accept new effect
		EffectsToTick.Add(NewTickingEffect.TickID, NewTickingEffect);
		CurrentEffectClasses.Add(IncomingClass);
	} else
	{
		// Replace old ticking effect with new one
		EffectsToTick.Add(GetTickingEffectIndex(IncomingClass), NewTickingEffect);
	}
	
	// Activate the Apply_Once effects only once when added to the tick container 
	if(IncomingEffect->GetEffectInitializationData().TickInterval == EEffectTickInterval::Apply_Once)
	{
		Internal_ActivateEffect(NewTickingEffect);
	}
	Internal_TryStartTicking();
}

void UEffectContainerComponent::Internal_ActivateEffect(const FTickingEffect& IncomingEffect)
{
	if (!IncomingEffect.TickingEffect)
	{
		return;
	}
	IncomingEffect.TickingEffect->TryActivateEffect();
}

void UEffectContainerComponent::Internal_DestroyEffect(TScriptInterface<IEffect> IncomingEffect, int32 TickID)
{
	if (!IncomingEffect)
	{
		return;
	}

	if (EffectsToTick.Contains(TickID))
	{
		EffectsToTick.Remove(TickID);	
	}
	
	const UClass* EffectClass = IncomingEffect.GetObject()->GetClass();

	// If CurrentEffectToTick don't have this Effect class anymore, remove it from the tracked Set
	if(!HasEffectClassAlready(EffectClass))
	{
		CurrentEffectClasses.Remove(EffectClass);
	}

	if (EffectsToTick.Num() <= 0)
	{
		Internal_StopTicking();
	}
	IncomingEffect->DestroyEffect();
}

double UEffectContainerComponent::ConvertIntervalToNumTicks(EEffectTickInterval EffectInterval)
{
	switch (EffectInterval)
	{
	case EEffectTickInterval::Apply_Every_Quarter_Second: return .25;
	case EEffectTickInterval::Apply_Every_Half_Second: return .5;
	case EEffectTickInterval::Apply_Every_Second: return 1;
	case EEffectTickInterval::Apply_Every_Two_Seconds: return 2;
	case EEffectTickInterval::Apply_Every_Three_Seconds: return 3;
	case EEffectTickInterval::Apply_Every_Five_Seconds: return 5;
	default: return -1;
	}
}

int32 UEffectContainerComponent::GenerateNumTicks(EEffectTickInterval EffectInterval, double Duration)
{
	const double Interval = ConvertIntervalToNumTicks(EffectInterval);
	double Remainder = 0;
	const int32 NumTicks = UKismetMathLibrary::FMod(Duration, Interval, Remainder);
	return NumTicks;
}

int32 UEffectContainerComponent::GenerateModulus(EEffectTickInterval EffectInterval)
{
	switch (EffectInterval)
	{
	case EEffectTickInterval::Apply_Every_Quarter_Second: return 1;
	case EEffectTickInterval::Apply_Every_Half_Second: return 2;
	case EEffectTickInterval::Apply_Every_Second: return 8;
	case EEffectTickInterval::Apply_Every_Two_Seconds: return 16;
	case EEffectTickInterval::Apply_Every_Three_Seconds: return 24;
	case EEffectTickInterval::Apply_Every_Five_Seconds: return 40;
	default: return -1;
	}
}
