#include "Ability/Activation/HitscanActivation.h"

#include "NiagaraFunctionLibrary.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/CommonCombatUtils.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInteractUtils.h"

void UHitscanActivation::Fire(int32 ActivationLevel)
{
	Internal_FireShot();
}

void UHitscanActivation::Internal_FireShot()
{
	const FHitResult& Impact = WeaponTrace(ShouldLineTrace(), 20.f);
	K2_ProcessInstantHit(Impact);
}

void UHitscanActivation::K2_ProcessInstantHit_Implementation(const FHitResult& Impact)
{
	const AActor* HitActor = Impact.GetActor();
	K2_PlayTrailFX(HitActor ? Impact.ImpactPoint : Impact.TraceEnd);
	if(!HitActor || UCommonInteractUtils::AreActorsAllies(HitActor, GetOwner()))
	{
		return;
	}

	if(!HitActor->FindComponentByClass<UEffectContainerComponent>())
	{
		Internal_PlayWeaponMissEffectFX(Impact);
	} else
	{
		UCommonEffectUtils::ApplyEffectsToHitResult(AbilityEffects, AdjustHitResultIfNoValidHitComponent(Impact), GetInstigator());
	}
}

void UHitscanActivation::Internal_PlayWeaponMissEffectFX(const FHitResult& Impact)
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

