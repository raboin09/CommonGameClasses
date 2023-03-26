// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EffectContainerComponent.h"

#include "Actors/CommonEffect.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Types/CoreTypes.h"
#include "Utils/CombatUtils.h"
#include "Utils/WorldUtils.h"

DECLARE_CYCLE_STAT(TEXT("CommonClasses_EffectContainerTick"), STAT_CommonClasses_StatsEffectContainerTicks, STATGROUP_StatSystem);

UEffectContainerComponent::UEffectContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEffectContainerComponent::TryApplyEffectToContainerFromHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult)
{
	if(!BaseEffectClass || !BaseEffectClass->ImplementsInterface(UEffect::StaticClass()))
	{
		return;
	} 
	const TScriptInterface<IEffect> BaseEffect = CreateEffectInstanceFromHitResult(this, BaseEffectClass, Impact, InstigatingActor, bShouldRotateHitResult);
	Internal_ApplyEffect(BaseEffect);
}

void UEffectContainerComponent::TryApplyEffectToContainer(TSubclassOf<AActor> BaseEffectClass, AActor* InstigatingActor)
{
	if(!BaseEffectClass || !BaseEffectClass->ImplementsInterface(UEffect::StaticClass()))
	{
		return;
	}
	const TScriptInterface<IEffect> BaseEffect = CreateEffectInstance(BaseEffectClass, InstigatingActor);
	Internal_ApplyEffect(BaseEffect);
}

TScriptInterface<IEffect> UEffectContainerComponent::CreateEffectInstanceFromHitResult(UObject* ContextObject, TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult)
{
	if(!ContextObject)
	{
		return nullptr;
	}
	
	FTransform SpawnTransform = FTransform(Impact.Location);
	if (bShouldRotateHitResult)
	{
		SpawnTransform = FTransform(UCombatUtils::GetRotationFromComponentHit(Impact), Impact.ImpactPoint);
	}

	ACommonEffect* EffectActor = UWorldUtils::SpawnActorToWorld_Deferred<ACommonEffect>(ContextObject, BaseEffectClass,InstigatingActor, Cast<APawn>(InstigatingActor));
	if (!EffectActor)
	{
		return nullptr;
	}

	FEffectContext EffectContext;
	EffectContext.InstigatingActor = InstigatingActor;
	EffectContext.ReceivingActor = Impact.GetActor();
	EffectContext.SurfaceHit = Impact;
	EffectContext.HitDirection = Impact.ImpactNormal;
	EffectActor->SetEffectContext(EffectContext);
	UWorldUtils::FinishSpawningActor_Deferred(EffectActor, SpawnTransform);
	return EffectActor;
}

void UEffectContainerComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedWorld = GetWorld();
}

TScriptInterface<IEffect> UEffectContainerComponent::CreateEffectInstance(TSubclassOf<AActor> BaseEffectClass, AActor* InstigatingActor) const
{
	if (!GetOwner())
	{
		return nullptr;
	}

	const FTransform& SpawnTransform = GetOwner()->GetActorTransform();

	ACommonEffect* EffectActor = UWorldUtils::SpawnActorToWorld_Deferred<ACommonEffect>(GetOwner(), BaseEffectClass);
	if (!EffectActor)
	{
		return nullptr;
	}

	EffectActor->SetInstigator(Cast<APawn>(InstigatingActor));
	FEffectContext EffectContext;
	EffectContext.InstigatingActor = InstigatingActor;
	EffectContext.ReceivingActor = GetOwner();
	EffectActor->SetEffectContext(EffectContext);
	UWorldUtils::FinishSpawningActor_Deferred(EffectActor, SpawnTransform);
	return EffectActor;
}

void UEffectContainerComponent::Internal_ApplyEffect(TScriptInterface<IEffect> IncomingEffect)
{
	if (!IncomingEffect)
	{
		return;
	}
	const FEffectInitializationData& EffectInitData = IncomingEffect->GetEffectInitializationData();

	if(EffectInitData.bInfinite)
	{
		Internal_AddEffectToTickContainer(IncomingEffect);
	} else
	{
		switch (EffectInitData.EffectInterval)
		{
		case EEffectInterval::Instant:
			{
				Internal_TryActivateEffect(IncomingEffect);
				Internal_DestroyEffect(IncomingEffect, -1);
				break;
			}
		case EEffectInterval::Apply_Once:
		case EEffectInterval::Apply_Every_Quarter_Second:
		case EEffectInterval::Apply_Every_Half_Second:
		case EEffectInterval::Apply_Every_Second:
		case EEffectInterval::Apply_Every_Two_Seconds:
		case EEffectInterval::Apply_Every_Three_Seconds:
		case EEffectInterval::Apply_Every_Five_Seconds:
			{
				Internal_AddEffectToTickContainer(IncomingEffect);
				break;
			}
		default: ;
		}
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

void UEffectContainerComponent::Internal_TryActivateEffect(TScriptInterface<IEffect> IncomingEffect)
{
	if(!IncomingEffect)
	{
		return;
	}
	
	Internal_RemoveEffectsWithTags(IncomingEffect->GetRemoveEffectTags(), IncomingEffect);
	if (CanApplyEffect(IncomingEffect))
	{
		IncomingEffect->ActivateEffect();
	}
}

void UEffectContainerComponent::Internal_TickEffects()
{
	SCOPE_CYCLE_COUNTER(STAT_CommonClasses_StatsEffectContainerTicks);
	if (++TickCounter > TotalTicksPerCycle)
	{
		TickCounter = 1;
	}

	for (const int32 CurrentTickingEffectKey : GetKeys())
	{
		Internal_TickEffect(CurrentTickingEffectKey);
	}
}

void UEffectContainerComponent::Internal_TickEffect(int32 CurrentTickingEffectKey)
{
	if(!CachedWorld || !EffectsToTick.Contains(CurrentTickingEffectKey))
		return;
	
	const TScriptInterface<IEffect> CurrentEffect = EffectsToTick[CurrentTickingEffectKey].TickingEffect;
	if(!CurrentEffect || CurrentEffect->GetEffectInitializationData().EffectInterval == EEffectInterval::Apply_Once)
		return;
	
	const bool bIsInfinite = CurrentEffect->GetEffectInitializationData().bInfinite;
	if (!bIsInfinite)
	{
		if(CachedWorld->GetTimeSeconds() >= EffectsToTick[CurrentTickingEffectKey].ExpirationTime)
		{
			Internal_DestroyEffect(CurrentEffect, CurrentTickingEffectKey);
			return;
		}
	}
	if (TickCounter % EffectsToTick[CurrentTickingEffectKey].TickModulus == 0){
			Internal_TryActivateEffect(CurrentEffect);
	}
}

void UEffectContainerComponent::Internal_StartTicking()
{
	if(bIsTicking || !CachedWorld)
	{
		return;
	}	
	CachedWorld->GetTimerManager().SetTimer(Timer_EffectTicker, this, &UEffectContainerComponent::Internal_TickEffects, QuarterSecondTick, true);
	bIsTicking = true;
}

void UEffectContainerComponent::Internal_StopTicking()
{
	if (!CachedWorld)
	{
		return;
	}
	CachedWorld->GetTimerManager().ClearTimer(Timer_EffectTicker);
	bIsTicking = false;
}

bool UEffectContainerComponent::CanApplyEffect(TScriptInterface<IEffect> IncomingEffect) const
{
	if (!IncomingEffect)
	{
		return false;
	}
	
	if (UGameplayTagComponent::ActorHasAnyGameplayTags(GetOwner(), IncomingEffect->GetBlockedTags()))
	{
		return false;
	}

	if (!UGameplayTagComponent::ActorHasAllGameplayTags(GetOwner(), IncomingEffect->GetRequiredTags()))
	{
		return false;
	}
	return true;
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
	TickingEffect.TickModulus = ConvertInterval(EffectInitializationData.EffectInterval);
	TickingEffect.TickID = TickIDCounter++;
	if(!IncomingEffect->GetEffectInitializationData().bInfinite)
	{
		TickingEffect.ExpirationTime = CachedWorld->GetTimeSeconds() + EffectInitializationData.EffectDuration;	
	}	
	TickingEffect.TickingEffect = IncomingEffect;
	return TickingEffect;
}

void UEffectContainerComponent::Internal_AddEffectToTickContainer(TScriptInterface<IEffect> IncomingEffect)
{
	if (!IncomingEffect || !CachedWorld)
	{
		return;
	}
	const UClass* IncomingClass = IncomingEffect.GetObject()->GetClass();
	const FTickingEffect NewTickingEffect = Internal_GenerateTickingEffectStruct(IncomingEffect);
	const bool bEffectCanStack = IncomingEffect->GetEffectInitializationData().bEffectCanStack; 
	if(bEffectCanStack || !HasEffectClassAlready(IncomingClass))
	{
		// TickingEffects can accept new effect
		EffectsToTick.Add(NewTickingEffect.TickID, NewTickingEffect);
		CurrentEffectClasses.Add(IncomingEffect.GetObject()->GetClass());
		IncomingEffect->ActivateEffect();
	} else
	{
		EffectsToTick.Add(GetTickingEffectIndex(IncomingClass), NewTickingEffect);
		// Replace old ticking effect with new one
	}
	Internal_StartTicking();
}

void UEffectContainerComponent::Internal_ActivateEffect(const FTickingEffect& IncomingEffect)
{
	if (!IncomingEffect.TickingEffect)
	{
		return;
	}
	IncomingEffect.TickingEffect->ActivateEffect();
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

int32 UEffectContainerComponent::ConvertInterval(EEffectInterval EffectInterval)
{
	switch (EffectInterval)
	{
	case EEffectInterval::Apply_Every_Quarter_Second: return 1;
	case EEffectInterval::Apply_Every_Half_Second: return 2;
	case EEffectInterval::Apply_Every_Second: return 4;
	case EEffectInterval::Apply_Every_Two_Seconds: return 8;
	case EEffectInterval::Apply_Every_Three_Seconds: return 12;
	case EEffectInterval::Apply_Every_Five_Seconds: return 20;
	default: return -1;
	}
}
