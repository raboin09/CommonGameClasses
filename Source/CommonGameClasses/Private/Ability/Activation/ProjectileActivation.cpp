// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Activation/ProjectileActivation.h"
#include "Actors/CommonProjectile.h"
#include "Core/ActorSystems/CommonSpawnSubsystem.h"

UProjectileActivation::UProjectileActivation()
{
	bHasFiringSpread = false;
}

void UProjectileActivation::Fire(const FTriggerEventPayload& TriggerEventPayload)
{
	HandleProjectileFire(TriggerEventPayload);
}

ACommonProjectile* UProjectileActivation::HandleProjectileFire(const FTriggerEventPayload& TriggerEventPayload)
{
	FVector Origin, ProjectileVelocity;
	Internal_AimAndShootProjectile(Origin, ProjectileVelocity);
	return Internal_SpawnProjectile(Origin, ProjectileVelocity, TriggerEventPayload);
}

TSubclassOf<ACommonProjectile> UProjectileActivation::BPN_GetProjectileClassToSpawn_Implementation(const FTriggerEventPayload& TriggerEventPayload) const
{
	return DefaultProjectileClass;
}

void UProjectileActivation::Internal_AimAndShootProjectile(FVector& OutSpawnOrigin, FVector& ProjectileVelocity)
{
	OutSpawnOrigin = GetRaycastOriginLocation();
	if (const FHitResult Impact = WeaponTrace(bShouldLineTrace, SphereTraceRadius)[0]; Impact.bBlockingHit)
	{
		const FVector AdjustedDir = (Impact.ImpactPoint - OutSpawnOrigin).GetSafeNormal();
		bool bWeaponPenetration = false;
		
		if (const float DirectionDot = FVector::DotProduct(AdjustedDir, ProjectileVelocity); DirectionDot < 0.0f)
		{
			bWeaponPenetration = true;
		}
		else if (DirectionDot < 0.5f)
		{
			const FVector MuzzleStartTrace = OutSpawnOrigin - GetRaycastOriginRotation() * 25.0f;
			const FVector MuzzleEndTrace = OutSpawnOrigin;
			if (const FHitResult MuzzleImpact = WeaponTrace(bShouldLineTrace, SphereTraceRadius, MuzzleStartTrace, MuzzleEndTrace)[0]; MuzzleImpact.bBlockingHit)
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

		if(bDrawDebugTrace)
		{
			DrawDebugLine(GetInstigator()->GetWorld(), Impact.TraceStart, Impact.TraceEnd, FColor::Red, false, .5f, 0, 1.f);
		}
	}
}

ACommonProjectile* UProjectileActivation::Internal_SpawnProjectile(const FVector& SpawnOrigin, const FVector& ProjectileVelocity, const FTriggerEventPayload& TriggerEventPayload)
{
	const TSubclassOf<ACommonProjectile> ProjectileClassToSpawn = BPN_GetProjectileClassToSpawn(TriggerEventPayload);
	FTransform SpawnTrans = FTransform();
	SpawnTrans.SetLocation(SpawnOrigin);
	if (ACommonProjectile* Projectile = UCommonSpawnSubsystem::SpawnActorToCurrentWorld_Deferred<ACommonProjectile>(this, ProjectileClassToSpawn, GetOwner(), GetInstigator(), ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn))
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
		UCommonSpawnSubsystem::FinishSpawningActor_Deferred(Projectile, SpawnTrans);
		return Projectile;
	}
	return nullptr;
}
