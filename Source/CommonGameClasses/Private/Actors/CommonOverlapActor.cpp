// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CommonOverlapActor.h"
#include "Components/ShapeComponent.h"

ACommonOverlapActor::ACommonOverlapActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bDiesAfterOverlap = false;
	DeathBuffer = 0;
}

void ACommonOverlapActor::BeginPlay()
{
	Super::BeginPlay();
	if(UShapeComponent* ShapeComponent = GetCollisionComponent())
	{
		ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ACommonOverlapActor::ActorBeginOverlap);
		ShapeComponent->OnComponentEndOverlap.AddDynamic(this, &ACommonOverlapActor::ActorEndOverlap);
		ShapeComponent->IgnoreActorWhenMoving(this, true);
		ShapeComponent->IgnoreComponentWhenMoving(GetMesh(), true);
	}

	if(bActivateOnStart)
	{
		UGameplayTagComponent::AddTagToActor(this, CommonGameState::Active);
	}

	// Add ACTIVE tag to activate at start
	if(!UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Active))
	{
		Deactivate();
	} else
	{
		Activate();
	}
}

void ACommonOverlapActor::K2_HandleOverlapEvent_Implementation(AActor* OtherActor, const FHitResult& HitResult)
{
	if(bDiesAfterOverlap) {
		HandleActorDeath();
	}
}

void ACommonOverlapActor::K2_HandleEndOverlapEvent_Implementation(AActor* ExitingActor)
{
	
}

void ACommonOverlapActor::HandleActorDeath()
{
	Deactivate();
	SetLifeSpan(DeathBuffer);
}

void ACommonOverlapActor::Activate()
{
	HitActors.Empty();
	UGameplayTagComponent::AddTagToActor(this, CommonGameState::Active);
	if(UShapeComponent* ShapeComponent = GetCollisionComponent())
	{
		ShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ShapeComponent->SetGenerateOverlapEvents(true);
	}
	K2_HandleActivation();
}

void ACommonOverlapActor::Deactivate()
{
	UGameplayTagComponent::RemoveTagFromActor(this, CommonGameState::Active);
	if(UShapeComponent* ShapeComponent = GetCollisionComponent())
	{
		ShapeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShapeComponent->SetGenerateOverlapEvents(false);
	}
	HitActors.Empty();
	K2_HandleDeactivation();
}

void ACommonOverlapActor::ActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || OtherActor->IsA(StaticClass()))
	{
		return;
	}

	const bool bTagsBlocked = BlockedOverlapTags.Num() > 0 ? UGameplayTagComponent::ActorHasAnyGameplayTags(OtherActor, BlockedOverlapTags) : false;
	if(bTagsBlocked)
	{
		return;
	}

	const bool bValidRequiredTags = RequiredOverlapTags.Num() > 0 ? UGameplayTagComponent::ActorHasAnyGameplayTags(OtherActor, RequiredOverlapTags) : true;
	if (IsActive() && !HitActors.Contains(OtherActor) && bValidRequiredTags) {
		HitActors.Add(OtherActor);
		K2_HandleOverlapEvent(OtherActor, SweepResult);
	}
}

void ACommonOverlapActor::ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && !OtherActor->IsA(StaticClass()) && HitActors.Contains(OtherActor))
	{
		HitActors.Remove(OtherActor);
		K2_HandleEndOverlapEvent(OtherActor);
	}
}