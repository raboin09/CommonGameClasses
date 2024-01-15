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
	
	virtual void GiveResource(const float AmountToGive) override;
	virtual bool TrySpendResource(const float RequestedAmount) override;
	virtual float CalculateResourceCost(const float RequestedAmount) const override;
	
protected:
	// Ammo be default doesn't regenerate (unless it has a heat mechanism where overheating is reaching 0 resources)
	FORCEINLINE virtual bool CanRegen() const override { return bAmmoRegenerates; }

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bInfiniteAmmo = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bInfiniteClip = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	bool bAmmoRegenerates = false;

	UPROPERTY(BlueprintAssignable)
	FAmmoAmountChangedEvent AmmoAmountChanged;
	UPROPERTY(BlueprintAssignable)
	FOutOfAmmoEvent OutOfAmmo;
	
private:
	void BroadcastAmmoUsage();

public:
	FORCEINLINE FAmmoAmountChangedEvent& OnAmmoAmountChanged() { return AmmoAmountChanged; }
	FORCEINLINE FOutOfAmmoEvent& OnOutOfAmmo() { return OutOfAmmo; }
};
