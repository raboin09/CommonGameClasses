// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnergyComponent.h"
#include "ShieldEnergyComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UShieldEnergyComponent : public UEnergyComponent
{
	GENERATED_BODY()

public:
	// Take shield damage, return any damage that wasn't applied (so if RawDamage was 50 but only 20 shield is
	// available, return 30.
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	float TakeShieldDamage(const float RawDamage, AActor* InstigatingActor, const FDamageHitReactEvent& HitReactEvent);
	
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	void ApplyShieldHeal(const float RawHeal, AActor* InstigatingActor = nullptr);
};
