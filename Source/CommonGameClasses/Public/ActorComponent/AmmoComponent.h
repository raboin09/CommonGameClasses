// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceComponent.h"
#include "Types/CommonEventDeclarations.h"
#include "AmmoComponent.generated.h"


struct FResourceContainerDefaults;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UAmmoComponent : public UResourceComponent
{
	GENERATED_BODY()

public:
	UAmmoComponent();
	virtual void BeginPlay() override;
	
	virtual void TryGiveResourceAmount(const float AmountToGive) override;
	virtual bool TryConsumeResourceAmount(const float RequestedAmount) override;
	virtual float CalculateConsumptionAmount(const float RequestedAmount) const override;
	
protected:
	// Ammo be default doesn't regenerate (unless it has a heat mechanism where overheating is reaching 0 resources)
	FORCEINLINE virtual bool CanRegen() const override { return bAmmoRegenerates; }

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Resources")
	bool bInfiniteAmmo = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Resources")
	bool bInfiniteClip = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Resources")
	bool bAmmoRegenerates = false;

	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Ammo Amount Changed")
	FAmmoAmountChangedEvent AmmoAmountChanged;
	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Out Of Ammo")
	FOutOfAmmoEvent OutOfAmmo;
	
private:
	void BroadcastAmmoUsage();

public:
	FORCEINLINE FAmmoAmountChangedEvent& OnAmmoAmountChanged() { return AmmoAmountChanged; }
	FORCEINLINE FOutOfAmmoEvent& OnOutOfAmmo() { return OutOfAmmo; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Resources")
	FORCEINLINE float GetAvailableAmmo() const { return GetAvailableResourceAmount(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON|Resources")
	FORCEINLINE float GetAmmoInClip() const { return GetMaxResourceAmount(); }
};
