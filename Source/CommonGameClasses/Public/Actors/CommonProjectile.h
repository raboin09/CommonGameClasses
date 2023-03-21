// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "GameFramework/Actor.h"
#include "CommonProjectile.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonProjectile : public ACommonActor
{
	GENERATED_BODY()


public:
	ACommonProjectile();
	virtual void PostInitializeComponents() override;
	void IgnoreActor(AActor* InActor) const;
	void InitVelocity(const FVector& ShootDirection) const;

	FORCEINLINE void AddAdditionalEffectsToApply(TArray<TSubclassOf<AActor>> AdditionalEffectsToApply) { ProjectileEffectsToApply.Append(AdditionalEffectsToApply);}

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON")
	void K2_HandleImpact(const FHitResult& HitResult);
	virtual void K2_HandleImpact_Implementation(const FHitResult& HitResult);
	UFUNCTION(BlueprintImplementableEvent)
	void K2_HandleActorDeath();
	UFUNCTION()
	virtual void OnImpact(const FHitResult& HitResult);
	virtual void HandleActorDeath();
	void ApplyMissEffects(const FHitResult Impact);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="COMMON|Defaults")
	class UProjectileMovementComponent* MovementComp;
	UPROPERTY(VisibleDefaultsOnly, Category="COMMON|Defaults")
	UParticleSystemComponent* ParticleComp;
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* CollisionComp;
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* SummonedMesh;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Defaults", meta=(MustImplement="Effect"))
	TArray<TSubclassOf<AActor>> ProjectileEffectsToApply;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Defaults")
	float DeathBuffer = 0.f;
};
