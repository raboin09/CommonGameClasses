// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ResourceComponent.h"

#include "Types/CommonCoreTypes.h"

UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();
	ResourcePoolContainer.InitResourceContainer(StartingResourcesPerPool, NumberOfResourcePools);
	Internal_StartRegenTimer();	
}

bool UResourceComponent::CanRegen() const
{
	return RegenAmount > 0.f && RegenRate > 0.f;
}

bool UResourceComponent::TrySpendResource(const float RequestedAmount)
{
	if(!CanSpendResourceAmount(RequestedAmount))
	{
		return false;
	}
	const float Delta = CalculateResourceCost(RequestedAmount);
	const float ActualResourcesTaken = ResourcePoolContainer.ConsumeResources(Delta);
	if(!bIsRegenTicking)
	{
		Internal_StartRegenTimer();	
	}
	return ActualResourcesTaken > 0;
}

void UResourceComponent::GiveResource(const float AmountToGive)
{
	const float Delta = AmountToGive;
	ResourcePoolContainer.GiveResources(Delta);
	if(ResourcePoolContainer.AreAllPoolsFull() && bIsRegenTicking)
	{
		Internal_StopRegenTimer();
	}
}

bool UResourceComponent::CanSpendResourceAmount(const float RequestedAmount)
{
	return ResourcePoolContainer.HasResources() && ResourcePoolContainer.GetSumOfAllResourcePools() > RequestedAmount;
}

void UResourceComponent::Internal_StartRegenTimer()
{
	if(!CanRegen())
	{
		return;
	}
	bIsRegenTicking = true;
	GetWorld()->GetTimerManager().SetTimer(Timer_RegenRate, this, &UResourceComponent::Internal_TickRegen, RegenRate, true);
}

void UResourceComponent::Internal_StopRegenTimer()
{
	if(!CanRegen())
	{
		return;
	}
	bIsRegenTicking = false;
	GetWorld()->GetTimerManager().ClearTimer(Timer_RegenRate);
}

void UResourceComponent::Internal_TickRegen()
{
	GiveResource(RegenAmount);
}
