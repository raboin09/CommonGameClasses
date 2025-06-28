// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/ResourceContainer.h"
#include "API/Core/SavableComponent.h"
#include "Components/ActorComponent.h"
#include "Types/CommonResourceTypes.h"
#include "ResourceComponent.generated.h"


struct FResourceContainerDefaults;

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UResourceComponent : public UActorComponent, public IResourceContainer, public ISavableComponent
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
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void InitResourceContainer(const float InStartingResourcePerPool, const int32 InNumResourcePools);

protected:
	virtual void BeginPlay() override;

	virtual bool CanRegen() const;
	virtual bool CanConsumeResourceAmount(const float RequestedAmount);
	// If there are any modifiers to consider (e.g. if this is HP damage, it will decrease RequestedAmount by half if
	// the Owner has an "Armored" status.
	virtual float CalculateConsumptionAmount(const float RequestedAmount) const PURE_VIRTUAL(UResourceComponent::CalculateConsumptionAmount, return 0.f;)

	UPROPERTY(EditAnywhere, SaveGame)
	int32 NumberOfResourcePools = 1;
	UPROPERTY(EditAnywhere, SaveGame)
	float StartingResourcesPerPool = 100.f;
	// Give RegenAmount every RegenRate seconds
	UPROPERTY(EditAnywhere, SaveGame)
	float RegenAmount = 0.f;
	UPROPERTY(EditAnywhere, SaveGame)
	float RegenRate = 1.f;
	
	UPROPERTY(SaveGame)
	FResourcePoolContainer ResourcePoolContainer;

private:
	FTimerHandle Timer_RegenRate;
	UPROPERTY(Transient)
	bool bIsRegenTicking = false;
	
	void Internal_StartRegenTimer();
	void Internal_StopRegenTimer();
	UFUNCTION()
	void Internal_TickRegen();
};