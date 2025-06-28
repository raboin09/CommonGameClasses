// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CommonDestructibleActor.h"

#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/InteractionComponent.h"
#include "..\..\Public\Types\CommonInteractTypes.h"
#include "Types/CommonTagTypes.h"


ACommonDestructibleActor::ACommonDestructibleActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DestructibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestructibleMesh"));
	DestructibleMesh->SetCollisionObjectType(ECC_WorldDynamic);
	DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DestructibleMesh->SetCollisionResponseToChannels(ECR_Block);
	SetRootComponent(DestructibleMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EffectContainerComponent = CreateDefaultSubobject<UEffectContainerComponent>(TEXT("EffectContainer"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->Affiliation = EAffiliation::Destructible;
}

void ACommonDestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	if(HealthComponent)
	{
		HealthComponent->OnActorDeath().AddDynamic(this, &ThisClass::HandleDeathEvent);
	}
}

void ACommonDestructibleActor::HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload)
{
	BPI_HandleInteractionStarted(InteractionEventPayload);
}

void ACommonDestructibleActor::HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload)
{
	BPI_HandleInteractionInitiated(InteractionEventPayload);
}

void ACommonDestructibleActor::HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload)
{
	BPI_HandleMeshOutlining(InteractionOutlineEventPayload);
}

void ACommonDestructibleActor::HandleDeathEvent(const FActorDeathEventPayload& EventPayload)
{
	UGameplayTagComponent::AddTagToActor(this, CommonGameState::Dead);
	if(InteractionComponent)
	{
		InteractionComponent->SwitchOutlineOnAllMeshes(false);	
	}
	BPI_HandleDeath(EventPayload.HitResult);
	SetLifeSpan(DeathBuffer);
}
