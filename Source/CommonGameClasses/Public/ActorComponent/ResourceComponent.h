// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/ResourceContainer.h"
#include "Components/ActorComponent.h"
#include "Types/ResourceTypes.h"
#include "ResourceComponent.generated.h"


struct FResourceContainerDefaults;

UCLASS(Abstract, NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UResourceComponent : public UActorComponent, public IResourceContainer
{
	GENERATED_BODY()

public:
	UResourceComponent();
	
	///////////////////////////////////
	/// IResourceContainer overrides
	////////////////////////////////////
	virtual bool TrySpendResource(const float RequestedAmount) override;
	virtual void GiveResource(const float AmountToGive) override;

protected:
	virtual void BeginPlay() override;
	
	virtual bool CanSpendResourceAmount(const float RequestedAmount);
	virtual float CalculateResourceCost(const float RequestedAmount) const PURE_VIRTUAL(UResourceComponent::CalculateResourceCost, return 0.f;)

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	int32 NumberOfResourcePools = 1;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float StartingResourcesPerPool = 100.f;
	// Give RegenAmount every RegenRate seconds
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float RegenAmount = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float RegenRate = 1.f;
	
	UPROPERTY()
	FResourcePoolContainer ResourcePoolContainer;

private:
	FTimerHandle Timer_RegenRate;
	UPROPERTY()
	bool bIsRegenTicking = false;
	
	void Internal_StartRegenTimer();
	void Internal_StopRegenTimer();
	UFUNCTION()
	void Internal_TickRegen();
};