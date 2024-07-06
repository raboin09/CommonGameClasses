﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/ResourceContainer.h"
#include "Components/ActorComponent.h"
#include "Types/CommonResourceTypes.h"
#include "ResourceComponent.generated.h"


struct FResourceContainerDefaults;

UCLASS(Abstract, DefaultToInstanced, EditInlineNew, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UResourceComponent : public UActorComponent, public IResourceContainer
{
	GENERATED_BODY()

public:
	UResourceComponent();
	
	///////////////////////////////////
	/// IResourceContainer overrides
	////////////////////////////////////
	virtual bool TryConsumeResourceAmount(const float RequestedAmount) override;
	virtual void TryGiveResourceAmount(const float AmountToGive) override;

	FORCEINLINE virtual float GetAvailableResourceAmount() const override { return ResourcePoolContainer.GetSumOfAllResourcePools(); }
	FORCEINLINE virtual float GetMaxResourceAmount() const override { return ResourcePoolContainer.GetMaxSumOfAllResourcePools(); }

protected:
	virtual void BeginPlay() override;

	virtual bool CanRegen() const;
	virtual bool CanConsumeResourceAmount(const float RequestedAmount);
	// If there are any modifiers to consider (e.g. if this is HP damage, it will decrease RequestedAmount by half if
	// the Owner has an "Armored" status.
	virtual float CalculateConsumptionAmount(const float RequestedAmount) const PURE_VIRTUAL(UResourceComponent::CalculateConsumptionAmount, return 0.f;)

	UPROPERTY(EditAnywhere)
	int32 NumberOfResourcePools = 1;
	UPROPERTY(EditAnywhere)
	float StartingResourcesPerPool = 100.f;
	// Give RegenAmount every RegenRate seconds
	UPROPERTY(EditAnywhere)
	float RegenAmount = 1.f;
	UPROPERTY(EditAnywhere)
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