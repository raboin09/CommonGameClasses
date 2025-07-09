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
	if(UShapeComponent* ShapeComponent = BPN_GetCollisionComponent())
	{
		ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ActorBeginOverlap);
		ShapeComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::ActorEndOverlap);
		ShapeComponent->IgnoreActorWhenMoving(this, true);
		ShapeComponent->IgnoreComponentWhenMoving(BPN_GetMesh(), true);
	}

	if(bDetectOverlapsOnBeginPlay)
	{
		UGameplayTagComponent::AddTagToActor(this, CommonGameState::Active);
	}

	// Add ACTIVE tag to activate at start
	if(!UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Active))
	{
		StopDetectingOverlaps();
	} else
	{
		StartDetectingOverlaps();
	}
}

bool ACommonOverlapActor::BPN_CanActorTriggerOverlap_Implementation(AActor* OtherActor)
{
	return true; 
}

void ACommonOverlapActor::HandleValidActorBeginOverlap(AActor* OtherActor, const FHitResult& HitResult)
{
	BPI_OnValidActorBeginOverlap(OtherActor, HitResult);
	if(bDiesAfterOverlap) {
		HandleActorDeath();
	}
}

void ACommonOverlapActor::HandleValidActorEndOverlap(AActor* ExitingActor)
{
	BPI_OnValidActorEndOverlap(ExitingActor);
}

void ACommonOverlapActor::HandleActorDeath()
{
	StopDetectingOverlaps();
	if(DeathBuffer > 0)
	{
		SetLifeSpan(DeathBuffer);	
	} else
	{
		Destroy();
	}
}

void ACommonOverlapActor::StartDetectingOverlaps()
{
	HitActors.Empty();
	UGameplayTagComponent::AddTagToActor(this, CommonGameState::Active);
	if(UShapeComponent* ShapeComponent = BPN_GetCollisionComponent())
	{
		ShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ShapeComponent->SetGenerateOverlapEvents(true);
	}
	BPI_OnStartDetectingOverlaps();
}

void ACommonOverlapActor::StopDetectingOverlaps()
{
	UGameplayTagComponent::RemoveTagFromActor(this, CommonGameState::Active);
	if(UShapeComponent* ShapeComponent = BPN_GetCollisionComponent())
	{
		ShapeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShapeComponent->SetGenerateOverlapEvents(false);
	}
	HitActors.Empty();
	BPI_OnStopDetectingOverlaps();
}

void ACommonOverlapActor::ActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsDetectingOverlaps())
	{
		return;
	}
	
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
	if(!bValidRequiredTags)
	{
		return;
	}

	if(!BPN_CanActorTriggerOverlap(OtherActor))
	{
		return;
	}
	
	if (!HitActors.Contains(OtherActor)) {
		HitActors.Add(OtherActor);
		HandleValidActorBeginOverlap(OtherActor, SweepResult);
	}
}

void ACommonOverlapActor::ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && !OtherActor->IsA(StaticClass()) && HitActors.Contains(OtherActor))
	{
		HitActors.Remove(OtherActor);
		HandleValidActorEndOverlap(OtherActor);
	}
}