#include "ActorComponent/EnergyComponent.h"

UEnergyComponent::UEnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnergyComponent::BeginPlay()
{
	Super::BeginPlay();
	BroadcastEnergyChangedEvent();
}

bool UEnergyComponent::TrySpendResource(const float RequestedAmount)
{
	if(Super::TrySpendResource(RequestedAmount))
	{
		BroadcastEnergyChangedEvent();
		return true;
	}
	return false;
}

void UEnergyComponent::GiveResource(const float AmountToGive)
{
	Super::GiveResource(AmountToGive);
	BroadcastEnergyChangedEvent();
}

void UEnergyComponent::BroadcastEnergyChangedEvent() const
{
	const float CurrentEnergy = ResourcePoolContainer.GetSumOfAllResourcePools();
	const float MaxEnergy = ResourcePoolContainer.GetMaxSumOfAllResourcePools();
	const FEnergyChangedEventPayload EnergyEventPayload = FEnergyChangedEventPayload(CurrentEnergy, MaxEnergy);
	EnergyAmountChanged.Broadcast(EnergyEventPayload);
}

float UEnergyComponent::CalculateResourceCost(const float RequestedAmount) const
{
	return RequestedAmount;
}
