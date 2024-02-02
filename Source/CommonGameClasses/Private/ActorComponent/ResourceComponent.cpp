// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ResourceComponent.h"

#include "ActorComponent/GameplayTagComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonTagTypes.h"

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

bool UResourceComponent::TryConsumeResourceAmount(const float RequestedAmount)
{
	if(!CanConsumeResourceAmount(RequestedAmount))
	{
		return false;
	}
	const float Delta = CalculateConsumptionAmount(RequestedAmount);
	const float ActualResourcesTaken = ResourcePoolContainer.ConsumeResources(Delta);
	if(!bIsRegenTicking && !UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameState::Dead))
	{
		Internal_StartRegenTimer();	
	}
	return ActualResourcesTaken > 0;
}

void UResourceComponent::TryGiveResourceAmount(const float AmountToGive)
{
	const float Delta = AmountToGive;
	ResourcePoolContainer.GiveResources(Delta);
	if(ResourcePoolContainer.AreAllPoolsFull() && bIsRegenTicking)
	{
		Internal_StopRegenTimer();
	}
}

bool UResourceComponent::CanConsumeResourceAmount(const float RequestedAmount)
{
	return ResourcePoolContainer.HasResources() && ResourcePoolContainer.GetSumOfAllResourcePools() >= RequestedAmount;
}

void UResourceComponent::Internal_StartRegenTimer()
{
	if(!CanRegen())
	{
		return;
	}
	bIsRegenTicking = true;
	GetWorld()->GetTimerManager().SetTimer(Timer_RegenRate, this, &ThisClass::Internal_TickRegen, RegenRate, true);
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
	TryGiveResourceAmount(RegenAmount);
}
