// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ShieldEnergyComponent.h"

float UShieldEnergyComponent::TakeShieldDamage(const float RawDamage, AActor* InstigatingActor, const FDamageHitReactEvent& HitReactEvent)
{
	float AppliedDamage = RawDamage;
	// If damage is greater than current energy, do that damage instead
	if(!CanConsumeResourceAmount(RawDamage))
	{
		AppliedDamage = GetCurrentEnergy();
	}
	TryConsumeResourceAmount(AppliedDamage);
	// Return any spillover damage (if there's un-applied damage, it needs to be applied to HP)
	return FMath::Max(RawDamage - AppliedDamage, 0.f);
}

void UShieldEnergyComponent::ApplyShieldHeal(const float RawHeal, AActor* InstigatingActor)
{
	TryGiveResourceAmount(RawHeal);
}
