﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceComponent.h"
#include "Types/CommonEventDeclarations.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UHealthComponent : public UResourceComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void BeginPlay() override;
	
	virtual float CalculateResourceCost(const float RequestedAmount) const override;
	virtual bool TrySpendResource(const float RequestedAmount) override;
	virtual void GiveResource(const float AmountToGive) override;
	
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	float TakeDamage(const float RawDamage, AActor* InstigatingActor, const FDamageHitReactEvent& HitReactEvent);
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void ApplyHeal(const float RawHeal, AActor* InstigatingActor = nullptr);
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void AddMaxWounds(float MaxWoundsToAdd);

protected:
	UPROPERTY(BlueprintAssignable)
	FCurrentWoundHealthChanged CurrentHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FMaxWoundsChanged MaxWoundsChanged;
	UPROPERTY(BlueprintAssignable)
	FActorDeath ActorDeath;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON")
	FORCEINLINE float GetCurrentHealth() const { return ResourcePoolContainer.GetSumOfAllResourcePools(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON")
	FORCEINLINE float GetMaxHealth() const { return ResourcePoolContainer.GetMaxSumOfAllResourcePools(); }
	
	FORCEINLINE FMaxWoundsChanged& OnMaxWoundsChanged() { return MaxWoundsChanged; }
	FORCEINLINE FActorDeath& OnActorDeath() { return ActorDeath; }
	FORCEINLINE FCurrentWoundHealthChanged& OnCurrentWoundHealthChanged() { return CurrentHealthChanged; }
};
