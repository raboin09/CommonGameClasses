﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AmmoComponent.h"


UAmmoComponent::UAmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();
	BroadcastAmmoUsage();
}

void UAmmoComponent::TryGiveResourceAmount(const float AmountToGive)
{
	Super::TryGiveResourceAmount(AmountToGive);
	BroadcastAmmoUsage();
}

bool UAmmoComponent::TryConsumeResourceAmount(const float RequestedAmount)
{
	if(Super::TryConsumeResourceAmount(RequestedAmount))
	{
		BroadcastAmmoUsage();
		return true;
	}
	return false;
}

float UAmmoComponent::CalculateConsumptionAmount(const float RequestedAmount) const
{
	return RequestedAmount;
}

void UAmmoComponent::BroadcastAmmoUsage()
{
	const int32 CurrentAmmoInClip = ResourcePoolContainer.GetCurrentResourcePool().CurrentResources;
	const int32 CurrentAmmo = ResourcePoolContainer.GetSumOfAllResourcePools();
	const int32 AmmoPerClip = ResourcePoolContainer.GetCurrentResourcePool().MaxResourcesInPool;
	const int32 TotalAmmo = ((( CurrentAmmo - CurrentAmmoInClip ) / AmmoPerClip) * AmmoPerClip) + ( CurrentAmmo - CurrentAmmoInClip ) % AmmoPerClip;
	AmmoAmountChanged.Broadcast(FAmmoAmountChangedPayload(CurrentAmmoInClip, AmmoPerClip, bInfiniteAmmo ? -1 : TotalAmmo, bInfiniteAmmo ? -1 : (AmmoPerClip * ResourcePoolContainer.NumPoolsInContainer)));
}
