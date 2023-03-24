﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	void K2_HandleImpact(const FHitResult& HitResult);
	virtual void K2_HandleImpact_Implementation(const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON")
	void K2_HandleActorDeath();
	UFUNCTION()
	virtual void OnImpact(const FHitResult& HitResult);
	virtual void HandleActorDeath();
	void ApplyMissEffects(const FHitResult Impact);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="CUSTOM|Defaults")
	UProjectileMovementComponent* MovementComp;
	UPROPERTY(VisibleDefaultsOnly, Category="CUSTOM|Defaults")
	UParticleSystemComponent* ParticleComp;
	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* CollisionComp;
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* SummonedMesh;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults", meta=(MustImplement="Effect"))
	TArray<TSubclassOf<AActor>> ProjectileEffectsToApply;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float DeathBuffer = 0.f;

public:
	FORCEINLINE void AddAdditionalEffectsToApply(TArray<TSubclassOf<AActor>> AdditionalEffectsToApply) { ProjectileEffectsToApply.Append(AdditionalEffectsToApply);}
};
