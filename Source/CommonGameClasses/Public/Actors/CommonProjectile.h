// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "GameFramework/Actor.h"
#include "CommonProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonProjectile : public ACommonActor
{
	GENERATED_BODY()


public:
	ACommonProjectile();
	virtual void PostInitializeComponents() override;
	void IgnoreActor(AActor* InActor) const;
	void InitVelocity(const FVector& ShootDirection) const;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON")
	void K2N_HandleImpact(const FHitResult& HitResult);
	virtual void K2N_HandleImpact_Implementation(const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void K2_HandleActorDeath();
	UFUNCTION()
	virtual void OnImpact(const FHitResult& HitResult);
	virtual void HandleActorDeath();
	void ApplyMissEffects(const FHitResult Impact);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> MovementComp;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UParticleSystemComponent> ParticleComp;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> CollisionComp;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> SummonedMesh;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Projectile", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> ProjectileEffectsToApply;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Projectile")
	float DeathBuffer = 0.f;

public:
	FORCEINLINE void AddAdditionalEffectsToApply(const TArray<TSubclassOf<AActor>>& AdditionalEffectsToApply) { ProjectileEffectsToApply.Append(AdditionalEffectsToApply);}
};
