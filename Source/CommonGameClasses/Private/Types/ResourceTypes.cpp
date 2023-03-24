// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/ResourceTypes.h"

void FResourcePoolContainer::ZeroizeResourceContainer()
{
	for(FResourcePool& CurrWound : ResourcePools)
	{
		CurrWound.CurrentResources = 0;
	}
}

void FResourcePoolContainer::MaximizeResourceContainer()
{
	for(FResourcePool& CurrWound : ResourcePools)
	{
		CurrWound.CurrentResources = MaxResourcePerPool;
		CurrWound.MaxResourcesInPool = MaxResourcePerPool;
	}
}

float FResourcePoolContainer::ConsumeResources(float ResourcesToConsume)
{
	FResourcePool& CurrWound = GetCurrentResourcePool();
	if(ResourcesToConsume > CurrWound.CurrentResources)
	{
		const float Delta = ResourcesToConsume - CurrWound.CurrentResources;
		CurrWound.CurrentResources = 0;
		if(CurrWound.ResourcePoolIndex + 1 < ResourcePools.Num())
		{
			ResourcePools[CurrWound.ResourcePoolIndex + 1].CurrentResources -= Delta;
		}
		return Delta;
	}
	CurrWound.CurrentResources -= ResourcesToConsume;
	return ResourcesToConsume;
}

void FResourcePoolContainer::GiveResources(float ResourcesToGive)
{
	if(ResourcePools.Num() == 0)
		return;
	FResourcePool& CurrWound = GetCurrentResourcePool();
	if(ResourcesToGive + CurrWound.CurrentResources > CurrWound.MaxResourcesInPool)
	{
		const float Delta = CurrWound.MaxResourcesInPool - ResourcesToGive + CurrWound.CurrentResources;
		// Max out the current resource pool
		CurrWound.CurrentResources = CurrWound.MaxResourcesInPool;
		// Add delta to the next wound in the array (if one exists)
		if(CurrWound.ResourcePoolIndex - 1 >= 0)
		{
			ResourcePools[CurrWound.ResourcePoolIndex - 1].CurrentResources += Delta;	
		}
	} else
	{
		CurrWound.CurrentResources += ResourcesToGive;
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
	for(const FResourcePool& CurrWound : ResourcePools)
	{
		if(CurrWound.CurrentResources > 0)
		{
			TempSum += CurrWound.CurrentResources;
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
	
	for(FResourcePool& CurrWound : ResourcePools)
	{
		// If a Wound has 0 < X < 100 health, it's the Current Wound
		if(CurrWound.CurrentResources > 0 && !CurrWound.IsFullPool())
		{
			return CurrWound;
		}

		// If a Wound has 100 HP but the previous has 0, then it's the Current Wound (ex. 0|0|100|100 , then the third is our Current Wound)
		if(CurrWound.ResourcePoolIndex > 0 && ResourcePools[CurrWound.ResourcePoolIndex - 1].CurrentResources <= 0 && CurrWound.CurrentResources > 0)
		{
			return CurrWound;
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
		FResourcePool TempWound = FResourcePool(StartingResourcePerPool, i);
		ResourcePools.Add(TempWound);
	}
	MaxResourcePerPool = StartingResourcePerPool;
	NumPoolsInContainer = NumResourcePools;
}

void FResourcePoolContainer::AddToMaxResourcesPerPool(int32 MaxWoundsToAdd)
{
	for(int i=ResourcePools.Num() - 1; i < ResourcePools.Num() + MaxWoundsToAdd - 1; i++)
	{
		FResourcePool TempWound = FResourcePool();
		TempWound.CurrentResources = MaxResourcePerPool;
		TempWound.MaxResourcesInPool = MaxResourcePerPool;
		ResourcePools.Add(TempWound);
	}
	NumPoolsInContainer += MaxResourcePerPool;
}
