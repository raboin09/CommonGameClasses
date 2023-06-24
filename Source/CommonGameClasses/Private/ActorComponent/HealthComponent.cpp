// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCoreTypes.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	FCurrentWoundEventPayload WoundEventPayload;
	WoundEventPayload.NewWound = ResourcePoolContainer.GetCurrentResourcePool();
	WoundEventPayload.OldWound = ResourcePoolContainer.GetCurrentResourcePool();
	WoundEventPayload.MaxWounds = ResourcePoolContainer.NumPoolsInContainer;
	WoundEventPayload.Delta = 0.f;
	WoundEventPayload.Percentage = ResourcePoolContainer.GetCurrentResourcePool().GetResourcePoolPercent();
	WoundEventPayload.bNaturalChange = false;
	CurrentHealthChanged.Broadcast(WoundEventPayload);
}

float UHealthComponent::TakeDamage(const float RawDamage, AActor* InstigatingActor, const FDamageHitReactEvent& HitReactEvent)
{
	if(!ResourcePoolContainer.HasResources())
	{
		FCurrentWoundEventPayload WoundEventPayload;
		WoundEventPayload.DamageHitReactEvent = HitReactEvent;
		WoundEventPayload.bNaturalChange = true;
		CurrentHealthChanged.Broadcast(WoundEventPayload);	
		return 0.f;
	}

	const FResourcePool OldWound = ResourcePoolContainer.GetCurrentResourcePool();
	if(!CanSpendResourceAmount(RawDamage))
	{
		return false;
	}
	const float Delta = CalculateResourceCost(RawDamage);
	const float ActualResourcesTaken = ResourcePoolContainer.ConsumeResources(Delta);
	const FResourcePool NewWound = ResourcePoolContainer.GetCurrentResourcePool();
	if(ResourcePoolContainer.HasResources())
	{
		FCurrentWoundEventPayload WoundEventPayload;
		WoundEventPayload.InstigatingActor = InstigatingActor;
		WoundEventPayload.ReceivingActor = GetOwner();
		WoundEventPayload.NewWound = NewWound;
		WoundEventPayload.OldWound = OldWound;
		WoundEventPayload.MaxWounds = ResourcePoolContainer.NumPoolsInContainer;
		WoundEventPayload.Delta = ActualResourcesTaken;
		WoundEventPayload.Percentage = NewWound.GetResourcePoolPercent();
		WoundEventPayload.bNaturalChange = true;
		WoundEventPayload.bWasDamage = true;
		WoundEventPayload.DamageHitReactEvent = HitReactEvent;
		WoundEventPayload.DamageHitReactEvent.DamageTaken = ActualResourcesTaken;
		CurrentHealthChanged.Broadcast(WoundEventPayload);		
	} else
	{
		FActorDeathEventPayload DeathEventPayload;
		DeathEventPayload.DyingDamage = ActualResourcesTaken;
		DeathEventPayload.DyingActor = GetOwner();
		DeathEventPayload.KillingActor = InstigatingActor;
		DeathEventPayload.HitReactEvent = HitReactEvent;
		DeathEventPayload.HitResult = HitReactEvent.HitResult;
		ActorDeath.Broadcast(DeathEventPayload);
	}
	return ActualResourcesTaken;
}

void UHealthComponent::ApplyHeal(const float RawHeal, AActor* InstigatingActor)
{
	const float Delta = RawHeal;
	const FResourcePool OldWound = ResourcePoolContainer.GetCurrentResourcePool();
	ResourcePoolContainer.GiveResources(Delta);
	const FResourcePool NewWound = ResourcePoolContainer.GetCurrentResourcePool();
	FCurrentWoundEventPayload WoundEventPayload;
	WoundEventPayload.InstigatingActor = InstigatingActor;
	WoundEventPayload.ReceivingActor = GetOwner();
	WoundEventPayload.NewWound = NewWound;
	WoundEventPayload.OldWound = OldWound;
	WoundEventPayload.bWasDamage = false;
	WoundEventPayload.MaxWounds = ResourcePoolContainer.NumPoolsInContainer;
	WoundEventPayload.Delta = Delta;
	WoundEventPayload.Percentage = NewWound.GetResourcePoolPercent();
	WoundEventPayload.bNaturalChange = true;
	CurrentHealthChanged.Broadcast(WoundEventPayload);
}

void UHealthComponent::AddMaxWounds(float MaxWoundsToAdd)
{
	const float OldWounds = ResourcePoolContainer.NumPoolsInContainer;
	ResourcePoolContainer.AddToMaxResourcesPerPool(MaxWoundsToAdd);
	const float NewWounds = ResourcePoolContainer.NumPoolsInContainer;
	FMaxWoundsEventPayload MaxWoundsEventPayload;
	MaxWoundsEventPayload.Delta = NewWounds - OldWounds;
	MaxWoundsEventPayload.NewMaxWounds = NewWounds;
	MaxWoundsEventPayload.OldMaxWounds = OldWounds;
	MaxWoundsChanged.Broadcast(MaxWoundsEventPayload);	
}

float UHealthComponent::CalculateResourceCost(const float RequestedAmount) const
{
	return RequestedAmount;
}

bool UHealthComponent::TrySpendResource(const float RequestedAmount)
{
	const FResourcePool OldWound = ResourcePoolContainer.GetCurrentResourcePool();
	const float OldSum = ResourcePoolContainer.GetSumOfAllResourcePools();
	if(Super::TrySpendResource(RequestedAmount))
	{
		const FResourcePool NewWound = ResourcePoolContainer.GetCurrentResourcePool();
		FCurrentWoundEventPayload WoundEventPayload;
		WoundEventPayload.InstigatingActor = GetOwner();
		WoundEventPayload.ReceivingActor = GetOwner();
		WoundEventPayload.NewWound = NewWound;
		WoundEventPayload.OldWound = OldWound;
		WoundEventPayload.bWasDamage = false;
		WoundEventPayload.MaxWounds = ResourcePoolContainer.NumPoolsInContainer;
		WoundEventPayload.Delta = OldSum - ResourcePoolContainer.GetSumOfAllResourcePools();
		WoundEventPayload.Percentage = NewWound.GetResourcePoolPercent();
		WoundEventPayload.bNaturalChange = true;
		CurrentHealthChanged.Broadcast(WoundEventPayload);
		return true;
	}
	return false;
}

void UHealthComponent::GiveResource(const float AmountToGive)
{
	const FResourcePool OldWound = ResourcePoolContainer.GetCurrentResourcePool();
	const float OldSum = ResourcePoolContainer.GetSumOfAllResourcePools();
	Super::GiveResource(AmountToGive);
	const float Delta = OldSum - ResourcePoolContainer.GetSumOfAllResourcePools();
	const FResourcePool NewWound = ResourcePoolContainer.GetCurrentResourcePool();
	FCurrentWoundEventPayload WoundEventPayload;
	WoundEventPayload.InstigatingActor = GetOwner();
	WoundEventPayload.ReceivingActor = GetOwner();
	WoundEventPayload.NewWound = NewWound;
	WoundEventPayload.OldWound = OldWound;
	WoundEventPayload.bWasDamage = false;
	WoundEventPayload.MaxWounds = ResourcePoolContainer.NumPoolsInContainer;
	WoundEventPayload.Delta = Delta;
	WoundEventPayload.Percentage = NewWound.GetResourcePoolPercent();
	WoundEventPayload.bNaturalChange = true;
	CurrentHealthChanged.Broadcast(WoundEventPayload);
}
