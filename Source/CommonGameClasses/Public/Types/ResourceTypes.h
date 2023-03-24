// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.generated.h"

UENUM(BlueprintType)
enum class EHitReactType : uint8
{
	None,
	Knockback_Tiny,
	Knockback_VeryLight,
	Knockback_Light,
	Knockback_Medium,
	Knockback_Heavy,
	Knockback_VeryHeavy,
	Knockback_Huge,
	HitReact_Light,
	HitReact_Chainsaw
};

USTRUCT(BlueprintType)
struct FDamageHitReactEvent
{
	GENERATED_BODY()
	
	FVector HitDirection;
	float DamageTaken;
	FHitResult HitResult;
	EHitReactType HitReactType = EHitReactType::None;
	EHitReactType DeathReactType = EHitReactType::None;
};

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Resources")
	float CurrentResources = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Health")
	float MaxResourcesInPool = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Health")
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

	UPROPERTY()
	TArray<FResourcePool> ResourcePools;
	UPROPERTY()
	float MaxResourcePerPool;
	UPROPERTY()
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