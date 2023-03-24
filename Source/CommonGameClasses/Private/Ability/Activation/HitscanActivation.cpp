#include "Ability/Activation/HitscanActivation.h"

#include "NiagaraFunctionLibrary.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/CombatUtils.h"
#include "Utils/EffectUtils.h"

void AHitscanActivation::Fire(int32 ActivationLevel)
{
	Internal_FireShot();
}

void AHitscanActivation::Internal_FireShot()
{
	const FHitResult& Impact = WeaponTrace(ShouldLineTrace(), 20.f);
	K2_ProcessInstantHit(Impact);
}

void AHitscanActivation::K2_ProcessInstantHit_Implementation(const FHitResult& Impact)
{
	AActor* HitActor = Impact.GetActor();
	K2_PlayTrailFX(HitActor ? Impact.ImpactPoint : Impact.TraceEnd);
	if(!HitActor || UCombatUtils::AreActorsAllies(HitActor, GetOwner()))
	{
		return;
	}

	if(!HitActor->FindComponentByClass<UEffectContainerComponent>())
	{
		Internal_PlayWeaponMissEffectFX(Impact);
	} else
	{
		UEffectUtils::ApplyEffectsToHitResult(AbilityEffects, AdjustHitResultIfNoValidHitComponent(Impact), GetInstigator());
	}
}

void AHitscanActivation::Internal_PlayWeaponMissEffectFX(const FHitResult& Impact)
{
	for(const TSubclassOf<AActor> CurrEffectClass : AbilityEffects)
	{
		if(const TScriptInterface<IEffect> TempEffect = UEffectContainerComponent::CreateEffectInstanceFromHitResult(this, CurrEffectClass, Impact, GetInstigator()))
		{
			TempEffect->PlayEffectFX();
			if(AActor* EffectActor = Cast<AActor>(TempEffect.GetObject()))
			{
				EffectActor->SetLifeSpan(.5f);
			}
		}
	}
}

