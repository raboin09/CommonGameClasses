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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "COMMON|Resources")
	float CurrentResources = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "COMMON|Resources")
	float MaxResourcesInPool = 100.f;
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

	UPROPERTY(SaveGame)
	TArray<FResourcePool> ResourcePools;
	UPROPERTY(SaveGame)
	float MaxResourcePerPool;
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