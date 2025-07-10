// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonResourceTypes.generated.h"

USTRUCT(BlueprintType)
struct FResourcePool
{
	GENERATED_USTRUCT_BODY()

	FResourcePool(const float StartingResourcePerPool, const int32 PoolIndex)
	{
		CurrentResources = StartingResourcePerPool;
		MaxResourcesInPool = StartingResourcePerPool;
		ResourcePoolIndex = PoolIndex;
	}

	FResourcePool()
	{
		CurrentResources = 0.f;
		MaxResourcesInPool = 0.f;
		ResourcePoolIndex = 0;
	}

	/**
	 * Represents the current amount of resources available in a resource pool.
	 * This variable can be modified in the editor, accessed in Blueprints, and saved into a save game.
	 * It is commonly used to track the remaining resources for gameplay systems, such as ammo or energy.
	 *
	 * Category: COMMON|Resources
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "COMMON|Resources")
	float CurrentResources = 100.f;
	/**
	 * Represents the maximum amount of resources that can be stored in a resource pool.
	 * This variable is adjustable in the editor, accessible in Blueprints, and persists in save games.
	 * It is often used to define the capacity of resource pools for gameplay systems, like maximum ammo or energy.
	 *
	 * Category: COMMON|Resources
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "COMMON|Resources")
	float MaxResourcesInPool = 100.f;
	/**
	 * Represents the unique index associated with a specific resource pool.
	 * This variable can be modified in the editor, accessed in Blueprints, and saved into a save game.
	 * It is used to identify and differentiate between multiple resource pools in a system.
	 *
	 * Category: COMMON|Resources
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "COMMON|Resources")
	int32 ResourcePoolIndex = 0;
	
	float GetResourcePoolPercent() const
	{
		return CurrentResources / MaxResourcesInPool;
	}
	float IsFullPool() const { return CurrentResources == MaxResourcesInPool; }

	bool operator==(const FResourcePool& Other) const
	{
		return (Other.ResourcePoolIndex == ResourcePoolIndex);
	}
};

USTRUCT(BlueprintType)
struct FResourcePoolContainer
{
	GENERATED_USTRUCT_BODY()

	/**
	 * Represents a collection of resource pools, where each pool maintains a set of resources
	 * that can be tracked and managed independently. Each resource pool within the array
	 * includes its current resource value, maximum capacity, and a unique identifier for differentiation.
	 *
	 * This variable is saved into a save game to persist its state across sessions. It can be used
	 * for systems requiring multiple resource pools, such as managing separate ammo types, energy levels, or health pools.
	 *
	 * Category: COMMON|Resources
	 */
	UPROPERTY(SaveGame)
	TArray<FResourcePool> ResourcePools;
	/**
	 * Defines the maximum amount of resources that each resource pool in the container can store.
	 * This value is used to initialize and manage the capacity of individual resource pools
	 * and ensures consistency across all pools within the container. It is also considered
	 * during save and load operations to maintain the state of resource pools.
	 *
	 * Category: COMMON|Resources
	 */
	UPROPERTY(SaveGame)
	float MaxResourcePerPool;
	/**
	 * Specifies the number of resource pools contained within a resource pool container.
	 * This variable determines the total count of separate pools managed by the container,
	 * each capable of tracking and storing individual resource quantities. It is saved
	 * into a save game to persist its state across gameplay sessions.
	 *
	 * This value is used during initialization, consumption, and modification of the container's pools,
	 * allowing systems to dynamically adjust the number of resource pools as required.
	 *
	 * Category: COMMON|Resources
	 */
	UPROPERTY(SaveGame)
	int32 NumPoolsInContainer;
	
	void InitResourceContainer(const float StartingResourcePerPool, const int32 NumResourcePools);
	void AddToMaxResourcesPerPool(int32 MaxWoundsToAdd);
	
	void ZeroizeResourceContainer();
	void MaximizeResourceContainer();	
	FResourcePool& GetCurrentResourcePool();

	float ConsumeResources(float ResourcesToConsume);
	void GiveResources(float ResourcesToGive);
	bool HasResources() const;
	float GetSumOfAllResourcePools() const;
	FORCEINLINE float GetMaxSumOfAllResourcePools() const { return MaxResourcePerPool * NumPoolsInContainer; }
	FORCEINLINE bool AreAllPoolsFull() const { return GetSumOfAllResourcePools() >= GetMaxSumOfAllResourcePools(); }
	float GetResourcesPerPool() const { return MaxResourcePerPool; }

	FResourcePoolContainer()
	{
		ResourcePools = { FResourcePool() };
		MaxResourcePerPool = 100.f;
		NumPoolsInContainer = 1;
	}
};