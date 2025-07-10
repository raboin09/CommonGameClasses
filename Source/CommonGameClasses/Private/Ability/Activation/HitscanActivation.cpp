#include "Ability/Activation/HitscanActivation.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInteractUtils.h"

void UHitscanActivation::Fire(const FTriggerEventPayload& TriggerEventPayload)
{
	Internal_FireShot();
}

void UHitscanActivation::Internal_FireShot()
{
	const FHitResult& Impact = WeaponTrace(ShouldLineTrace(), TraceRadius);
	BPN_ProcessInstantHit(Impact);
}

void UHitscanActivation::BPN_ProcessInstantHit_Implementation(const FHitResult& Impact)
{
	const AActor* HitActor = Impact.GetActor();
	BPI_PlayTrailFX(Impact.TraceStart, HitActor ? Impact.ImpactPoint : Impact.TraceEnd);
	if(!HitActor || UCommonInteractUtils::AreActorsAllies(HitActor, GetInstigator()))
	{
		return;
	}

	if(!HitActor->FindComponentByClass<UEffectContainerComponent>())
	{
		Internal_PlayWeaponMissEffectFX(Impact);
	} else
	{
		BPI_OnSuccessfulHit(Impact);
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

