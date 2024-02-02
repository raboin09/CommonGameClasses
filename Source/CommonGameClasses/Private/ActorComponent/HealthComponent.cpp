// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"

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

void UHealthComponent::TakeDamage(const float RawDamage, AActor* InstigatingActor, const FDamageHitReactEvent& HitReactEvent)
{
	// If the owner is already dead, apply hit react but don't do any damage
	if(!ResourcePoolContainer.HasResources())
	{
		FCurrentWoundEventPayload WoundEventPayload;
		WoundEventPayload.DamageHitReactEvent = HitReactEvent;
		WoundEventPayload.bNaturalChange = true;
		CurrentHealthChanged.Broadcast(WoundEventPayload);
		return;
	}

	float ActualDamageToApply = RawDamage;
	// If incoming damage is NOT lethal, apply damage and return. Otherwise, broadcast death event.
	if(!CanConsumeResourceAmount(RawDamage))
	{
		ActualDamageToApply = GetCurrentHealth();
	}

	const FResourcePool OldWound = ResourcePoolContainer.GetCurrentResourcePool();
	TryConsumeResourceAmount(ActualDamageToApply);
	
	if(GetAvailableResourceAmount() <= 0)
	{
		FActorDeathEventPayload DeathEventPayload;
		DeathEventPayload.DyingDamage = RawDamage;
		DeathEventPayload.DyingActor = GetOwner();
		DeathEventPayload.KillingActor = InstigatingActor;
		DeathEventPayload.HitReactEvent = HitReactEvent;
		DeathEventPayload.HitResult = HitReactEvent.HitResult;
		ActorDeath.Broadcast(DeathEventPayload);	
	} else {
		const FResourcePool NewWound = ResourcePoolContainer.GetCurrentResourcePool();
		FCurrentWoundEventPayload WoundEventPayload;
		WoundEventPayload.InstigatingActor = InstigatingActor;
		WoundEventPayload.ReceivingActor = GetOwner();
		WoundEventPayload.NewWound = NewWound;
		WoundEventPayload.OldWound = OldWound;
		WoundEventPayload.MaxWounds = ResourcePoolContainer.NumPoolsInContainer;
		WoundEventPayload.Delta = ActualDamageToApply;
		WoundEventPayload.Percentage = NewWound.GetResourcePoolPercent();
		WoundEventPayload.bNaturalChange = true;
		WoundEventPayload.bWasDamage = true;
		WoundEventPayload.DamageHitReactEvent = HitReactEvent;
		WoundEventPayload.DamageHitReactEvent.DamageTaken = RawDamage;
		CurrentHealthChanged.Broadcast(WoundEventPayload);
	}
}

void UHealthComponent::ApplyHeal(const float RawHeal, AActor* InstigatingActor)
{
	const float Delta = RawHeal;
	const FResourcePool OldWound = ResourcePoolContainer.GetCurrentResourcePool();
	TryGiveResourceAmount(Delta);
	const FResourcePool NewWound = ResourcePoolContainer.GetCurrentResourcePool();
	if(Delta > 0)
	{
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
}

void UHealthComponent::AddMaxWounds(float MaxWoundsToAdd)
{
	const float OldWounds = ResourcePoolContainer.NumPoolsInContainer;
	ResourcePoolContainer.AddToMaxResourcesPerPool(MaxWoundsToAdd);
	const float NewWounds = ResourcePoolContainer.NumPoolsInContainer;
	const float Delta = NewWounds - OldWounds;
	if(Delta > 0)
	{
		FMaxWoundsEventPayload MaxWoundsEventPayload;
		MaxWoundsEventPayload.Delta = Delta;
		MaxWoundsEventPayload.NewMaxWounds = NewWounds;
		MaxWoundsEventPayload.OldMaxWounds = OldWounds;
		MaxWoundsChanged.Broadcast(MaxWoundsEventPayload);		
	}
}

float UHealthComponent::CalculateConsumptionAmount(const float RequestedAmount) const
{
	return RequestedAmount;
}
