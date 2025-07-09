// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/ResourceContainer.h"
#include "API/Core/SavableComponent.h"
#include "Components/ActorComponent.h"
#include "Types/CommonResourceTypes.h"
#include "ResourceComponent.generated.h"


struct FResourceContainerDefaults;

UCLASS(Abstract, ClassGroup=(Custom), HideCategories=("Variable", "Sockets", "Tags", "ComponentReplication", "Component Replication", "Cooking", "Replication", "AssetUserData", "Asset User Data", "Navigation"), meta=(BlueprintSpawnableComponent))
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
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	void InitResourceContainer(const float InStartingResourcePerPool, const int32 InNumResourcePools);

protected:
	virtual void BeginPlay() override;

	virtual bool CanRegen() const;
	virtual bool CanConsumeResourceAmount(const float RequestedAmount);

	/**
	 * Calculates the amount of resource that can actually be consumed based on the requested amount.
	 * This method must be implemented by derived classes and may account for various modifiers
	 * or conditions affecting the resource consumption, such as buffs, debuffs, or other gameplay mechanics.
	 *
	 * @param RequestedAmount The amount of resource requested for consumption.
	 * @return The calculated amount of resource to actually consume.
	 */
	virtual float CalculateConsumptionAmount(const float RequestedAmount) const PURE_VIRTUAL(UResourceComponent::CalculateConsumptionAmount, return 0.f;)

	/**
	 * The number of individual resource pools available within the resource container.
	 * Determines how many separate resource pools will be initialized and managed by the component.
	 *
	 * Default Value: 1
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category="COMMON|Resources")
	int32 NumberOfResourcePools = 1;
	/**
	 * Represents the default amount of resources allocated to each pool during initialization of the resource system.
	 * This value is used when setting up resource pools managed by the component.
	 *
	 * Default Value: 100.0
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category="COMMON|Resources")
	float StartingResourcesPerPool = 100.f;
	/**
	 * The amount of resources restored to the resource container during each regeneration tick.
	 * Determines how much the resource level increases at every interval when regeneration is active.
	 *
	 * Default Value: 0.0
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category="COMMON|Resources")
	float RegenAmount = 0.f;
	/**
	 * The interval, in seconds, at which resource regeneration occurs.
	 * This value determines the frequency of the regeneration system,
	 * impacting how often resources are restored to the resource container.
	 *
	 * Default Value: 1.0
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category="COMMON|Resources")
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