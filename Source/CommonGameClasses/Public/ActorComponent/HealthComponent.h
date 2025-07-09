// Fill out your copyright notice in the Description page of Project Settings.

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
	
	virtual float CalculateConsumptionAmount(const float RequestedAmount) const override;

	/**
	 * Applies damage to the health component, triggers events for health changes, and processes damage until death if lethal.
	 *
	 * @param RawDamage The raw amount of damage to be applied.
	 * @param InstigatingActor The actor responsible for instigating the damage.
	 * @param HitReactEvent Details about the hit reaction, including hit context and damage specifics.
	 */
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	void TakeDamage(const float RawDamage, AActor* InstigatingActor, const FDamageHitReactEvent& HitReactEvent);
	/**
	 * Applies healing to the health component, updates the health resource pool,
	 * and triggers events for any changes to the health state.
	 *
	 * @param RawHeal The raw amount of healing to be applied.
	 * @param InstigatingActor The actor responsible for initiating the healing.
	 */
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	void ApplyHeal(const float RawHeal, AActor* InstigatingActor = nullptr);
	/**
	 * Fully restores all health for the associated actor by maximizing the resource pool
	 * and triggers health change events if applicable.
	 *
	 * @param InstigatingActor The actor responsible for initiating the full healing action, can be null.
	 */
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	void ApplyFullHeal(AActor* InstigatingActor = nullptr);
	/**
	 * Increases the maximum number of wounds the health component can sustain,
	 * adjusts the internal resource pools, and broadcasts an event if the maximum value changes.
	 *
	 * @param MaxWoundsToAdd The amount to increase the maximum wounds by.
	 */
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	void AddMaxWounds(float MaxWoundsToAdd);

protected:
	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Current Health Changed")
	FCurrentWoundHealthChanged CurrentHealthChanged;
	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Max Wounds Changed")
	FMaxWoundsChanged MaxWoundsChanged;
	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Actor Death")
	FActorDeath ActorDeath;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Resources")
	FORCEINLINE float GetCurrentHealth() const { return GetAvailableResourceAmount(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Resources")
	FORCEINLINE float GetMaxHealth() const { return GetMaxResourceAmount(); }
	
	FORCEINLINE FMaxWoundsChanged& OnMaxWoundsChanged() { return MaxWoundsChanged; }
	FORCEINLINE FActorDeath& OnActorDeath() { return ActorDeath; }
	FORCEINLINE FCurrentWoundHealthChanged& OnCurrentWoundHealthChanged() { return CurrentHealthChanged; }
};
