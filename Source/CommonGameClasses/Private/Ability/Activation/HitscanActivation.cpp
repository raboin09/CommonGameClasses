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
	TArray<FHitResult> Impacts = WeaponTrace(bShouldLineTrace, SphereTraceRadius);
	if(Impacts.Num() == 0)
	{
		BPI_OnEmptyHitscan();
		return;
	}
	
	for(FHitResult& Impact : Impacts)
	{
		BPN_ProcessInstantHit(Impact);
	}
}

void UHitscanActivation::BPN_ProcessInstantHit_Implementation(const FHitResult& Impact)
{
	const AActor* HitActor = Impact.GetActor();
	BPI_PlayTrailFX(Impact.TraceStart, HitActor ? Impact.ImpactPoint : Impact.TraceEnd);
	if(!HitActor)
	{
		return;
	}

	if(ActivationEffectsToApply.Num() > 0 && !HitActor->FindComponentByClass<UEffectContainerComponent>())
	{
		BPI_OnHitscanMiss(Impact);
		Internal_PlayHitscanMissEffectFX(Impact);
	} else
	{
		BPI_OnHitscanSuccessfulHit(Impact);
		if(ActivationEffectsToApply.Num() > 0)
		{
			UCommonEffectUtils::ApplyEffectsToHitResult(AdjustHitResultIfNoValidHitComponent(Impact), ActivationEffectsToApply, GetInstigator());	
		}
	}
}

void UHitscanActivation::Internal_PlayHitscanMissEffectFX(const FHitResult& Impact)
{
	for(const TSubclassOf<AActor> CurrEffectClass : ActivationEffectsToApply)
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

