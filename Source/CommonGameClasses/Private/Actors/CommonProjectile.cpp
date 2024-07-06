// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Types/CommonCoreTypes.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInteractUtils.h"

ACommonProjectile::ACommonProjectile() 
{
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(COMMON_OBJECT_TYPE_PROJECTILE);
	CollisionComp->SetCollisionResponseToChannel(COMMON_OBJECT_TYPE_PROJECTILE, ECR_Ignore);
	CollisionComp->SetCollisionProfileName("BlockAllDynamic");
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->bTraceComplexOnMove = true;
	SetRootComponent(CollisionComp);

	SummonedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SummonedMesh"));
	SummonedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SummonedMesh->SetCollisionProfileName("BlockAllDynamic");
	SummonedMesh->SetupAttachment(RootComponent);
	SummonedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SummonedMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = true;
	ParticleComp->SetupAttachment(SummonedMesh);
	ParticleComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ParticleComp->LODMethod = PARTICLESYSTEMLODMETHOD_ActivateAutomatic;

	PrimaryActorTick.bCanEverTick = false;
}

void ACommonProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(MovementComp)
	{
		MovementComp->OnProjectileStop.AddDynamic(this, &ThisClass::OnImpact);
	}
}

void ACommonProjectile::IgnoreActor(AActor* InActor) const
{
	if(CollisionComp)
	{
		CollisionComp->IgnoreActorWhenMoving(InActor, true);
	}

	if(SummonedMesh)
	{
		SummonedMesh->IgnoreActorWhenMoving(InActor, true);
	}
}

void ACommonProjectile::InitVelocity(const FVector& ShootDirection) const
{
	if (MovementComp)
	{
		MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	}
}

void ACommonProjectile::K2N_HandleImpact_Implementation(const FHitResult& HitResult)
{
	UCommonEffectUtils::ApplyEffectsToHitResult(ProjectileEffectsToApply, HitResult, GetInstigator());
}

void ACommonProjectile::OnImpact(const FHitResult& HitResult)
{
	if(!HitResult.GetActor())
	{
		HandleActorDeath();
		return;
	}
	const UClass* HitActorClass = HitResult.GetActor()->GetClass();
	AActor* HitActor = HitResult.GetActor();
	if(!HitActorClass || !HitActor)
	{
		return;
	}

	if(HitActor->FindComponentByClass<UEffectContainerComponent>() && !UCommonInteractUtils::AreActorsAllies(HitActor, GetInstigator()))
	{
		K2N_HandleImpact(HitResult);
	} else
	{
		ApplyMissEffects(HitResult);
	}
	HandleActorDeath();
}

void ACommonProjectile::ApplyMissEffects(const FHitResult Impact)
{
	for(const TSubclassOf<AActor> CurrEffectClass : ProjectileEffectsToApply)
	{
		if(const TScriptInterface<IEffect> TempEffect = UEffectContainerComponent::CreateEffectInstanceFromHitResult(this, CurrEffectClass, Impact, GetInstigator(), true))
		{
			TempEffect->PlayEffectFX();
			TempEffect->DestroyEffect();
		}
	}
}

void ACommonProjectile::HandleActorDeath()
{
	K2_HandleActorDeath();
	if(SummonedMesh)
	{
		SummonedMesh->SetVisibility(false);
	}
	
	MovementComp->StopMovementImmediately();
	if(DeathBuffer > 0.f)
	{
		SetLifeSpan(DeathBuffer);	
	} else
	{
		Destroy();
	}
}