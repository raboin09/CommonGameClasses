// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonExplodingProjectile.h"
#include "Utils/CommonEffectUtils.h"

ACommonExplodingProjectile::ACommonExplodingProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACommonExplodingProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(!bExplodedAlready)
	{
		Internal_Explode();
	}
	Super::EndPlay(EndPlayReason);
}

void ACommonExplodingProjectile::HandleActorDeath()
{
	if(!bExplodedAlready)
	{
		Internal_Explode();
	}
	Super::HandleActorDeath();
}

void ACommonExplodingProjectile::BPN_HandleImpact_Implementation(const FHitResult& HitResult)
{
	Internal_Explode();
}

void ACommonExplodingProjectile::OnImpact(const FHitResult& HitResult)
{
	BPN_HandleImpact_Implementation(HitResult);
	HandleActorDeath();
}

void ACommonExplodingProjectile::Internal_Explode()
{
	bExplodedAlready = true;
	UCommonEffectUtils::ApplyEffectsInRadius(GetInstigator(), ProjectileEffectsToApply, GetActorLocation(), ExplosionRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), bCanFriendlyFire);
	BPI_HandleExplosion();
}
  