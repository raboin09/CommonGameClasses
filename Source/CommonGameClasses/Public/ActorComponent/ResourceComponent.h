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

	/**
	 * Tries to consume the requested amount of resource from the resource pool.
	 * The method ensures that the requested amount can be consumed first, accounts for applicable modifiers,
	 * invokes internal consumption mechanics, and may trigger regeneration if necessary.
	 *
	 * @param RequestedAmount The amount of resource requested for consumption.
	 * @return true if the requested resource amount could be partially or fully consumed, false otherwise.
	 */
	virtual bool TryConsumeResourceAmount(const float RequestedAmount) override;

	virtual bool CanConsumeResourceAmount(const float RequestedAmount) const override;
	
	/**
	 * Attempts to add the specified amount of resources to the resource container.
	 * This method modifies the resource pool by adding the given amount and performs
	 * additional logic, such as stopping regeneration if all resource pools are full.
	 *
	 * @param AmountToGive The amount of resources to attempt to add to the resource pools.
	 */
	virtual void TryGiveResourceAmount(const float AmountToGive) override;
	/**
	 * Toggles the pause state of the resource regeneration system.
	 * When paused, resource regeneration stops. When unpaused, regeneration resumes
	 * based on the component's internal mechanics.
	 *
	 * @param bShouldPause Indicates whether resource regeneration should be paused (true) or resumed (false).
	 */
	virtual void TryTogglePauseResourceRegen(bool bShouldPause) override;
	/**
	 * Retrieves the total amount of available resources from all resource pools within the resource container.
	 * This method calculates the sum of resources across all pools managed by the component.
	 *
	 * @return The total amount of resources currently available.
	 */
	FORCEINLINE virtual float GetAvailableResourceAmount() const override { return ResourcePoolContainer.GetSumOfAllResourcePools(); }
	/**
	 * Retrieves the maximum amount of resource available across all resource pools in the resource container.
	 * This method calculates the total maximum capacity by aggregating the maximum values
	 * of all individual resource pools managed by the component.
	 *
	 * @return The maximum total amount of resources across all resource pools.
	 */
	FORCEINLINE virtual float GetMaxResourceAmount() const override { return ResourcePoolContainer.GetMaxSumOfAllResourcePools(); }
	//~ End IResourceContainer Interface
	
	UFUNCTION(BlueprintCallable, Category = "COMMON|Resources")
	void InitResourceContainer(const float InStartingResourcePerPool, const int32 InNumResourcePools);

protected:
	virtual void BeginPlay() override;
	virtual bool CanRegen() const;

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