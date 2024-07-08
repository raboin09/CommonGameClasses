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
	K2N_ProcessInstantHit(Impact);
}

void UHitscanActivation::K2N_ProcessInstantHit_Implementation(const FHitResult& Impact)
{
	const AActor* HitActor = Impact.GetActor();
	K2_PlayTrailFX(Impact.TraceStart, HitActor ? Impact.ImpactPoint : Impact.TraceEnd);
	if(!HitActor || UCommonInteractUtils::AreActorsAllies(HitActor, GetInstigator()))
	{
		if(bDrawDebugTrace)
		{
			DrawDebugLine(GetInstigator()->GetWorld(), Impact.TraceStart, Impact.TraceEnd, FColor::Red, false, .5f, 0, 1.f);
		}
		return;
	}

	if(!HitActor->FindComponentByClass<UEffectContainerComponent>())
	{
		if(bDrawDebugTrace)
		{
			DrawDebugLine(GetInstigator()->GetWorld(), Impact.TraceStart, Impact.TraceEnd, FColor::Red, false, .5f, 0, 1.f);
		}
		Internal_PlayWeaponMissEffectFX(Impact);
	} else
	{
		if(bDrawDebugTrace)
		{
			DrawDebugLine(GetInstigator()->GetWorld(), Impact.TraceStart, Impact.TraceEnd, FColor::Green, false, .5f, 0, 1.f);	
		}
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

