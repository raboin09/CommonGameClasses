// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Activation/ProjectileActivation.h"
#include "Actors/CommonProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/WorldUtils.h"

AProjectileActivation::AProjectileActivation()
{
	bHasFiringSpread = false;
}

void AProjectileActivation::Fire(int32 ActivationLevel)
{
	HandleProjectileFire();
}

ACommonProjectile* AProjectileActivation::HandleProjectileFire()
{
	FVector Origin, ProjectileVelocity;
	Internal_AimAndShootProjectile(Origin, ProjectileVelocity);
	return Internal_SpawnProjectile(Origin, ProjectileVelocity);
}

void AProjectileActivation::Internal_AimAndShootProjectile(FVector& OutSpawnOrigin, FVector& ProjectileVelocity)
{
	OutSpawnOrigin = GetRaycastOriginLocation();
	constexpr float RaycastCircleRadius = 20.f;
	if (FHitResult Impact = WeaponTrace(ShouldLineTrace(), RaycastCircleRadius); Impact.bBlockingHit)
	{
		const FVector AdjustedDir = (Impact.ImpactPoint - OutSpawnOrigin).GetSafeNormal();
		bool bWeaponPenetration = false;
		
		if (const float DirectionDot = FVector::DotProduct(AdjustedDir, ProjectileVelocity); DirectionDot < 0.0f)
		{
			bWeaponPenetration = true;
		}
		else if (DirectionDot < 0.5f)
		{
			FVector MuzzleStartTrace = OutSpawnOrigin - GetRaycastOriginRotation() * 25.0f;
			FVector MuzzleEndTrace = OutSpawnOrigin;
			if (FHitResult MuzzleImpact = WeaponTrace(ShouldLineTrace(), RaycastCircleRadius, MuzzleStartTrace, MuzzleEndTrace); MuzzleImpact.bBlockingHit)
			{
				bWeaponPenetration = true;
			}
		}
		
		if (bWeaponPenetration)
		{
			OutSpawnOrigin = Impact.ImpactPoint - ProjectileVelocity * 10.0f;
		}
		else
		{
			ProjectileVelocity = AdjustedDir;
		}
	}
}

ACommonProjectile* AProjectileActivation::Internal_SpawnProjectile(const FVector& SpawnOrigin, const FVector& ProjectileVelocity)
{

	FTransform SpawnTrans = FTransform();
	SpawnTrans.SetLocation(SpawnOrigin);
	if (ACommonProjectile* Projectile = UWorldUtils::SpawnActorToCurrentStreamedWorld_Deferred<ACommonProjectile>(ProjectileClass, this, GetInstigator(), ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn))
	{		
		Projectile->InitVelocity(ProjectileVelocity);
		Projectile->SetLifeSpan(ProjectileLife);
		Projectile->AddAdditionalEffectsToApply(Internal_GetAdditionalEffectsToApplyToProjectile());
		Projectile->SetInstigator(GetInstigator());
		Projectile->SetOwner(GetInstigator());
		for(AActor* TempActor : GetActorsToIgnoreCollision())
		{
			Projectile->IgnoreActor(TempActor);
		}
		UWorldUtils::FinishSpawningActor_Deferred(Projectile, SpawnTrans);
		return Projectile;
	}
	return nullptr;
}
