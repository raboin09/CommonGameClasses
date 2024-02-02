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

bool UEnergyComponent::TryConsumeResourceAmount(const float RequestedAmount)
{
	if(Super::TryConsumeResourceAmount(RequestedAmount))
	{
		BroadcastEnergyChangedEvent();
		return true;
	}
	return false;
}

void UEnergyComponent::TryGiveResourceAmount(const float AmountToGive)
{
	Super::TryGiveResourceAmount(AmountToGive);
	BroadcastEnergyChangedEvent();
}

void UEnergyComponent::BroadcastEnergyChangedEvent() const
{
	const float CurrentEnergy = ResourcePoolContainer.GetSumOfAllResourcePools();
	const float MaxEnergy = ResourcePoolContainer.GetMaxSumOfAllResourcePools();
	const FEnergyChangedEventPayload EnergyEventPayload = FEnergyChangedEventPayload(CurrentEnergy, MaxEnergy);
	EnergyAmountChanged.Broadcast(EnergyEventPayload);
}

float UEnergyComponent::CalculateConsumptionAmount(const float RequestedAmount) const
{
	return RequestedAmount;
}
