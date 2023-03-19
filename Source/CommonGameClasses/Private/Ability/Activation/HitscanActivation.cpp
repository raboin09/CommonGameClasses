#include "Ability/Activation/HitscanActivation.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/EffectContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/CombatUtils.h"
#include "Utils/EffectUtils.h"

void AHitscanActivation::Fire()
{
	Internal_FireShot();
}

void AHitscanActivation::Internal_FireShot()
{
	const FVector& AimDirection = GetAdjustedAim();
	const FVector& StartTrace = GetCameraDamageStartLocation(AimDirection);
	const FVector ShootDirection = GetShootDirection(AimDirection);
	const FVector& EndTrace = StartTrace + ShootDirection * TraceRange;
	const FHitResult& Impact = WeaponTrace(StartTrace, EndTrace, ShouldLineTrace(), 20.f);
	Internal_ProcessInstantHit(Impact, StartTrace, ShootDirection);
}


void AHitscanActivation::Internal_ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDirection)
{
	AActor* HitActor = Impact.GetActor(); 
	const FVector EndTrace = Origin + ShootDirection * TraceRange;
	const FVector EndPoint = HitActor ? Impact.ImpactPoint : EndTrace;
	K2_PlayTrailFX(EndPoint);
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

