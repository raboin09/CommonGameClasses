﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CommonDestructibleActor.h"

#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/InteractionComponent.h"
#include "Types/CommonTypes.h"
#include "Types/TagTypes.h"


ACommonDestructibleActor::ACommonDestructibleActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DestructibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestructibleMesh"));
	DestructibleMesh->SetCollisionObjectType(ECC_WorldDynamic);
	DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DestructibleMesh->SetCollisionResponseToChannels(ECR_Block);
	DestructibleMesh->SetCollisionResponseToChannel(COMMON_OBJECT_TYPE_PROJECTILE, ECR_Block);
	SetRootComponent(DestructibleMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EffectContainerComponent = CreateDefaultSubobject<UEffectContainerComponent>(TEXT("EffectContainer"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void ACommonDestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	if(HealthComponent)
	{
		HealthComponent->OnActorDeath().AddUObject(this, &ACommonDestructibleActor::HandleDeathEvent);
	}
}

void ACommonDestructibleActor::HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload)
{
	K2_HandleInteractionStarted(InteractionEventPayload);
}

void ACommonDestructibleActor::HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload)
{
	K2_HandleInteractionInitiated(InteractionEventPayload);
}

void ACommonDestructibleActor::HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload)
{
	K2_HandleMeshOutlining(InteractionOutlineEventPayload);
}

void ACommonDestructibleActor::HandleDeathEvent(const FActorDeathEventPayload& EventPayload)
{
	UGameplayTagComponent::AddTagToActor(this, TAG_STATE_DEAD);
	if(InteractionComponent)
	{
		InteractionComponent->SwitchOutlineOnAllMeshes(false);	
	}
	K2_HandleDeath(EventPayload.HitResult);
	SetLifeSpan(DeathBuffer);
}
