// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CommonPlayerController.h"
#include "Character/CommonPlayerCharacter.h"
#include "API/Interactable.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponent/QuestManagerComponent.h"
#include "Utils/InteractUtils.h"

ACommonPlayerController::ACommonPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	QuestManager = CreateDefaultSubobject<UQuestManagerComponent>(TEXT("QuestManager"));
}

void ACommonPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Internal_TryAssignInteractable();
}

void ACommonPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<ACommonPlayerCharacter>(InPawn);
}

void ACommonPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	PlayerCharacter = nullptr;
}

void ACommonPlayerController::MoveToNewDestination(const FVector& MoveLocation)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MoveLocation);
}

void ACommonPlayerController::OnNewActorTargeted(const AActor* NewHoveredActor)
{
	if(!NewHoveredActor)
	{
		K2_HandleNewActorHovered(CurrentHoveredInteractionComponent, false);
		CurrentHoveredInteractionComponent = nullptr;
		return;
	}

	UInteractionComponent* FoundInteractionComponent = NewHoveredActor->FindComponentByClass<UInteractionComponent>();
	if(!FoundInteractionComponent || FoundInteractionComponent == CurrentHoveredInteractionComponent)
	{
		return;
	}

	K2_HandleNewActorHovered(CurrentHoveredInteractionComponent, false);
	CurrentHoveredInteractionComponent = FoundInteractionComponent;
	K2_HandleNewActorHovered(FoundInteractionComponent, true);
}

void ACommonPlayerController::Internal_CheckDistanceToInteractActor()
{
	if(IsInRangeOfInteractable(TargetedInteractionComponent))
	{
		PlayerCharacter->GetMovementComponent()->StopActiveMovement();
		TargetedInteractionComponent->InitiateInteraction(PlayerCharacter, true);
		Internal_ClearCheckDistTimer();
	} else
	{
		CachedDistanceCheckTime += DISTANCE_CHECK_RATE;
		if(CachedDistanceCheckTime >= MAX_DISTANCE_CHECK_TIME)
		{
			Internal_ClearCheckDistTimer();
		}
	}
}


void ACommonPlayerController::Internal_ClearCheckDistTimer()
{
	TargetedInteractionComponent = nullptr;
	CachedDistanceCheckTime = 0.f;
	GetWorldTimerManager().ClearTimer(Timer_InteractDistanceCheck);
}

void ACommonPlayerController::K2_HandleNewActorHovered_Implementation(const UInteractionComponent* NewlyHoveredInteractable, bool bShouldOutline)
{
	if(NewlyHoveredInteractable)
	{
		CurrentHoveredInteractionComponent->SwitchOutlineOnAllMeshes(bShouldOutline);	
	}
}

void ACommonPlayerController::K2_TryStartInteraction_Implementation()
{
	if(CurrentHoveredInteractionComponent)
	{
		if(IsInRangeOfInteractable(CurrentHoveredInteractionComponent))
		{
			CurrentHoveredInteractionComponent->InitiateInteraction(PlayerCharacter, true);	
		} else
		{
			TargetedInteractionComponent = CurrentHoveredInteractionComponent;
			MoveToNewDestination(TargetedInteractionComponent->GetOwnerLocation());
			GetWorldTimerManager().SetTimer(Timer_InteractDistanceCheck, this, &ACommonPlayerController::Internal_CheckDistanceToInteractActor, DISTANCE_CHECK_RATE, true);
		}
	}
}

void ACommonPlayerController::K2_StopInteraction_Implementation()
{
	if(CurrentHoveredInteractionComponent)
	{
		CurrentHoveredInteractionComponent->InitiateInteraction(PlayerCharacter, false);
		Internal_ClearCheckDistTimer();
	}
}

void ACommonPlayerController::Internal_TryAssignInteractable()
{
	const FHitResult& HitResult = Internal_ScanForActorUnderCursor();
	if(IsValidInteractableActorUnderCursor(HitResult))
	{
		OnNewActorTargeted(HitResult.GetActor());
	} else if(CurrentHoveredInteractionComponent)
	{
		OnNewActorTargeted(nullptr);
	}
}

bool ACommonPlayerController::IsValidInteractableActorUnderCursor(const FHitResult& HitResult) const
{
	const AActor* NewHoveredActor = HitResult.GetActor();
	if(!HitResult.bBlockingHit || !NewHoveredActor || NewHoveredActor == PlayerCharacter)
	{
		return false;
	}

	if(!NewHoveredActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return false;
	}
	return true;
}

FHitResult ACommonPlayerController::Internal_ScanForActorUnderCursor() const
{
	FHitResult TempResult;
	GetHitResultUnderCursor(COMMON_TRACE_INTERACTION, true, TempResult);
	return TempResult;
}

bool ACommonPlayerController::IsInRangeOfInteractable(const UInteractionComponent* InteractionComponent) const
{
	if (!PlayerCharacter || !InteractionComponent) {
		return false;
	}

	const float DistanceToProtagonist = FVector::Dist(InteractionComponent->GetOwnerLocation(), PlayerCharacter->GetActorLocation());
	switch (UInteractUtils::GetAffiliationOfActor(InteractionComponent->GetOwner()))
	{
	case EAffiliation::Allies:
	case EAffiliation::Neutral:
		if (DistanceToProtagonist < InteractWithAlliesDistance) {
			return true;
		}
		break;
	case EAffiliation::Enemies:
		if( DistanceToProtagonist < InteractWithEnemiesDistance) {
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

