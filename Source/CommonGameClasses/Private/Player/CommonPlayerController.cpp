// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CommonPlayerController.h"
#include "Character/CommonPlayerCharacter.h"
#include "API/Interactable.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponent/QuestManagerComponent.h"

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

void ACommonPlayerController::OnNewActorTargeted(AActor* NewHoveredActor)
{
	if(!NewHoveredActor)
	{
		K2_HandleNewActorHovered(CurrentHoveredInteractable, false);
		CurrentHoveredInteractable = nullptr;
		CurrentHoveredActor = nullptr;
		return;
	}
	
	TScriptInterface<IInteractable> NewHoveredInteractable;
	NewHoveredInteractable.SetObject(NewHoveredActor);
	NewHoveredInteractable.SetInterface(Cast<IInteractable>(NewHoveredActor));
	
	if(NewHoveredInteractable == CurrentHoveredInteractable)
	{
		return;
	}

	K2_HandleNewActorHovered(CurrentHoveredInteractable, false);
	CurrentHoveredInteractable = NewHoveredInteractable;
	CurrentHoveredActor = NewHoveredActor;
	K2_HandleNewActorHovered(NewHoveredInteractable, true);
}

void ACommonPlayerController::Internal_CheckDistanceToInteractActor()
{
	if(IsInRangeOfInteractable(TargetedInteractable, TargetedActor))
	{
		PlayerCharacter->GetMovementComponent()->StopActiveMovement();
		TargetedInteractable->InitiateInteractionWithActor(PlayerCharacter);
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
	TargetedInteractable = nullptr;
	TargetedActor = nullptr;
	CachedDistanceCheckTime = 0.f;
	GetWorldTimerManager().ClearTimer(Timer_InteractDistanceCheck);
}

void ACommonPlayerController::K2_HandleNewActorHovered_Implementation(const TScriptInterface<IInteractable>& NewlyHoveredInteractable, bool bShouldOutline)
{
	if(NewlyHoveredInteractable)
	{
		CurrentHoveredInteractable->SwitchOutlineOnMesh(bShouldOutline);	
	}
}

void ACommonPlayerController::K2_TryStartInteraction_Implementation()
{
	if(CurrentHoveredInteractable && CurrentHoveredActor)
	{
		if(IsInRangeOfInteractable(CurrentHoveredInteractable, CurrentHoveredActor))
		{
			CurrentHoveredInteractable->InitiateInteractionWithActor(PlayerCharacter, true);	
		} else
		{
			TargetedActor = CurrentHoveredActor;
			TargetedInteractable = CurrentHoveredInteractable;
			MoveToNewDestination(TargetedActor->GetActorLocation());
			GetWorldTimerManager().SetTimer(Timer_InteractDistanceCheck, this, &ACommonPlayerController::Internal_CheckDistanceToInteractActor, DISTANCE_CHECK_RATE, true);
		}
	}
}

void ACommonPlayerController::K2_StopInteraction_Implementation()
{
	if(CurrentHoveredInteractable && CurrentHoveredActor)
	{
		CurrentHoveredInteractable->InitiateInteractionWithActor(PlayerCharacter, false);
		Internal_ClearCheckDistTimer();
	}
}

void ACommonPlayerController::Internal_TryAssignInteractable()
{
	const FHitResult& HitResult = Internal_ScanForActorUnderCursor();
	if(IsValidInteractableActorUnderCursor(HitResult))
	{
		OnNewActorTargeted(HitResult.GetActor());
	} else if(CurrentHoveredInteractable)
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
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, TempResult);
	return TempResult;
}

bool ACommonPlayerController::IsInRangeOfInteractable(TScriptInterface<IInteractable> InInteractable, const AActor* InActor) const
{
	if (!InActor || !InInteractable || !PlayerCharacter) {
		return false;
	}

	const float DistanceToProtagonist = FVector::Dist(InActor->GetActorLocation(), PlayerCharacter->GetActorLocation());
	switch (InInteractable->GetAffiliation())
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

