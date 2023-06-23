// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/ResourceTypes.h"

void FResourcePoolContainer::ZeroizeResourceContainer()
{
	for(FResourcePool& CurrResourcePool : ResourcePools)
	{
		CurrResourcePool.CurrentResources = 0;
	}
}

void FResourcePoolContainer::MaximizeResourceContainer()
{
	for(FResourcePool& CurrResourcePool : ResourcePools)
	{
		CurrResourcePool.CurrentResources = MaxResourcePerPool;
		CurrResourcePool.MaxResourcesInPool = MaxResourcePerPool;
	}
}

float FResourcePoolContainer::ConsumeResources(float ResourcesToConsume)
{
	FResourcePool& CurrResourcePool = GetCurrentResourcePool();
	if(ResourcesToConsume > CurrResourcePool.CurrentResources)
	{
		const float Delta = ResourcesToConsume - CurrResourcePool.CurrentResources;
		CurrResourcePool.CurrentResources = 0;
		if(CurrResourcePool.ResourcePoolIndex + 1 < ResourcePools.Num())
		{
			ResourcePools[CurrResourcePool.ResourcePoolIndex + 1].CurrentResources -= Delta;
		}
		return Delta;
	}
	CurrResourcePool.CurrentResources -= ResourcesToConsume;
	return ResourcesToConsume;
}

void FResourcePoolContainer::GiveResources(float ResourcesToGive)
{
	if(ResourcePools.Num() == 0)
		return;
	FResourcePool& CurrResourcePool = GetCurrentResourcePool();
	if(ResourcesToGive + CurrResourcePool.CurrentResources > CurrResourcePool.MaxResourcesInPool)
	{
		const float Delta = CurrResourcePool.MaxResourcesInPool - ResourcesToGive + CurrResourcePool.CurrentResources;
		// Max out the current resource pool
		CurrResourcePool.CurrentResources = CurrResourcePool.MaxResourcesInPool;
		// Add delta to the next ResourcePool in the array (if one exists)
		if(CurrResourcePool.ResourcePoolIndex - 1 >= 0)
		{
			ResourcePools[CurrResourcePool.ResourcePoolIndex - 1].CurrentResources += Delta;	
		}
	} else
	{
		CurrResourcePool.CurrentResources += ResourcesToGive;
	}
}

bool FResourcePoolContainer::HasResources() const
{
	if(ResourcePools.Num() <= 0)
		return false;
	return ResourcePools[ResourcePools.Num() - 1].CurrentResources > 0;
}

float FResourcePoolContainer::GetSumOfAllResourcePools() const
{
	float TempSum = 0.f;
	for(const FResourcePool& CurrResourcePool : ResourcePools)
	{
		if(CurrResourcePool.CurrentResources > 0)
		{
			TempSum += CurrResourcePool.CurrentResources;
		}
	}
	return TempSum;
}

FResourcePool& FResourcePoolContainer::GetCurrentResourcePool()
{	
	// Check if dead
	if(ResourcePools[ResourcePools.Num() - 1].CurrentResources <= 0)
	{
		return ResourcePools[ResourcePools.Num() - 1];
	}
	
	for(FResourcePool& CurrResourcePool : ResourcePools)
	{
		// If a ResourcePool has 0 < X < 100 health, it's the Current Resource Pool
		if(CurrResourcePool.CurrentResources > 0 && !CurrResourcePool.IsFullPool())
		{
			return CurrResourcePool;
		}

		// If a ResourcePool has 100 HP but the previous has 0, then it's the Current Resource Pool (ex. 0|0|100|100 , then the third is our Current Resource Pool)
		if(CurrResourcePool.ResourcePoolIndex > 0 && ResourcePools[CurrResourcePool.ResourcePoolIndex - 1].CurrentResources <= 0 && CurrResourcePool.CurrentResources > 0)
		{
			return CurrResourcePool;
		}
	}
	
	// None of the conditions fit, default to full HP
	return ResourcePools[0];
}

void FResourcePoolContainer::InitResourceContainer(const float StartingResourcePerPool, const int32 NumResourcePools)
{
	ResourcePools.Empty();
	for(int i=0; i<NumResourcePools; i++)
	{
		FResourcePool TempResourcePool = FResourcePool(StartingResourcePerPool, i);
		ResourcePools.Add(TempResourcePool);
	}
	MaxResourcePerPool = StartingResourcePerPool;
	NumPoolsInContainer = NumResourcePools;
}

void FResourcePoolContainer::AddToMaxResourcesPerPool(int32 MaxResourcePoolsToAdd)
{
	for(int i=ResourcePools.Num() - 1; i < ResourcePools.Num() + MaxResourcePoolsToAdd - 1; i++)
	{
		FResourcePool TempResourcePool = FResourcePool();
		TempResourcePool.CurrentResources = MaxResourcePerPool;
		TempResourcePool.MaxResourcesInPool = MaxResourcePerPool;
		ResourcePools.Add(TempResourcePool);
	}
	NumPoolsInContainer += MaxResourcePerPool;
}
