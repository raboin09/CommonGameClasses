// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CommonPlayerController.h"

#include "ActorComponent/AbilityComponent.h"
#include "Character/CommonPlayerCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ActorComponent/InteractionComponent.h"
#include "API/Ability/Ability.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AbilityOutliner.h"
#include "Utils/CommonInteractUtils.h"

ACommonPlayerController::ACommonPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
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
	check(PlayerCharacter.IsValid())
	if(UAbilityComponent* AbilityComponent = PlayerCharacter->FindComponentByClass<UAbilityComponent>())
	{
		AbilityComponent->OnNewAbilityEquipped().AddUniqueDynamic(this, &ThisClass::HandleNewAbilityEquipped);
	}
}

void ACommonPlayerController::MoveToNewDestination(const FVector& MoveLocation)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MoveLocation);
}

void ACommonPlayerController::NewActorTargeted(const AActor* NewHoveredActor)
{
	if(!NewHoveredActor)
	{
		NewActorHovered(CurrentHoveredInteractionComponent.Get(), false);
		CurrentHoveredInteractionComponent.Reset();
		return;
	}

	UInteractionComponent* FoundInteractionComponent = NewHoveredActor->FindComponentByClass<UInteractionComponent>();
	if(!FoundInteractionComponent || FoundInteractionComponent == CurrentHoveredInteractionComponent)
	{
		return;
	}
	NewActorHovered(CurrentHoveredInteractionComponent.Get(), false);
	CurrentHoveredInteractionComponent = FoundInteractionComponent;
	NewActorHovered(FoundInteractionComponent, true);
}

void ACommonPlayerController::HandleNewAbilityEquipped(const FNewAbilityEquippedPayload& NewAbilityEquippedPayload)
{
	if(!NewAbilityEquippedPayload.NewEquippedAbility.IsValid())
	{
		ValidAbilityOutlineDistance = DEFAULT_OUTLINE_DISTANCE;
	} else
	{
		TWeakInterfacePtr<IAbility> AbilityWeakPtr = NewAbilityEquippedPayload.NewEquippedAbility.Get();
		if(UObject* AbilityObject = AbilityWeakPtr.GetObject(); const IAbilityOutliner* AbilityOutliner = Cast<IAbilityOutliner>(AbilityObject))
		{
			ValidAbilityOutlineDistance = AbilityOutliner->GetAbilityOutlineRange();
		} else
		{
			ValidAbilityOutlineDistance = DEFAULT_OUTLINE_DISTANCE;
		}
	}

	if(!CurrentHoveredInteractionComponent.IsValid())
	{
		return;
	}
		
	if(!IsValidInteractionComponent(CurrentHoveredInteractionComponent))
	{
		CurrentHoveredInteractionComponent->SwitchOutlineOnAllMeshes(false);
	}
}

void ACommonPlayerController::Internal_CheckDistanceToInteractActor()
{
	if(IsInRangeOfInteractable(TargetedInteractionComponent.Get()))
	{
		if(PlayerCharacter.IsValid() && PlayerCharacter->GetMovementComponent())
		{
			PlayerCharacter->GetMovementComponent()->StopActiveMovement();	
		}
		TargetedInteractionComponent->InitiateInteraction(PlayerCharacter.Get(), true);
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

void ACommonPlayerController::Internal_GetShooterCameraTargetedActor(FHitResult& HitResult) const
{
	// Remember to add a new class type if it needs to be considered alive via IsActorAlive()
	FVector StartLocation = FVector::ZeroVector;
	FRotator CamRot;
	GetPlayerViewPoint(StartLocation, CamRot);
	const FVector AimDir = CamRot.Vector();
	if(PlayerCharacter.IsValid())
	{
		const float TraceDistance = UKismetMathLibrary::Max(DEFAULT_OUTLINE_DISTANCE, ValidAbilityOutlineDistance);
		const FVector OutStartTrace = StartLocation + AimDir * ((PlayerCharacter->GetActorLocation() - StartLocation) | AimDir);
		const FVector OutEndTrace = OutStartTrace + AimDir * TraceDistance;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(PlayerCharacter.Get());
		const auto DrawDebug = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		UKismetSystemLibrary::SphereTraceSingle(this, OutStartTrace, OutEndTrace, InteractTraceRadius, UEngineTypes::ConvertToTraceType(COMMON_TRACE_INTERACTION),
			false, IgnoreActors, DrawDebug, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f);
	}
}

void ACommonPlayerController::NewActorHovered(UInteractionComponent* NewlyHoveredInteractable, bool bShouldOutline)
{
	if(!NewlyHoveredInteractable)
	{
		return;
	}
	NewlyHoveredInteractable->SwitchOutlineOnAllMeshes(bShouldOutline);
}

void ACommonPlayerController::TryStartInteractionWithCurrentInteractable()
{
	if(CurrentHoveredInteractionComponent.IsValid())
	{
		if(IsInRangeOfInteractable(CurrentHoveredInteractionComponent.Get()))
		{
			CurrentHoveredInteractionComponent->InitiateInteraction(PlayerCharacter.Get(), true);	
		} else
		{
			TargetedInteractionComponent = CurrentHoveredInteractionComponent;
			MoveToNewDestination(TargetedInteractionComponent->GetOwnerLocation());
			GetWorldTimerManager().SetTimer(Timer_InteractDistanceCheck, this, &ThisClass::Internal_CheckDistanceToInteractActor, DISTANCE_CHECK_RATE, true);
		}
	}
}

void ACommonPlayerController::StopInteraction()
{
	if(CurrentHoveredInteractionComponent.IsValid())
	{
		CurrentHoveredInteractionComponent->InitiateInteraction(PlayerCharacter.Get(), false);
		Internal_ClearCheckDistTimer();
	}
}

void ACommonPlayerController::Internal_TryAssignInteractable()
{
	const FHitResult& HitResult = Internal_ScanForTargetedActors();
	if(IsValidInteractionHitResult(HitResult))
	{
		NewActorTargeted(HitResult.GetActor());
	} else if(CurrentHoveredInteractionComponent.IsValid())
	{
		NewActorTargeted(nullptr);
	}
}

bool ACommonPlayerController::IsValidInteractionHitResult(const FHitResult& HitResult) const
{
	const AActor* NewHoveredActor = HitResult.GetActor();
	if(!HitResult.bBlockingHit || !NewHoveredActor || NewHoveredActor == PlayerCharacter)
	{
		return false;
	}

	const TWeakObjectPtr<UInteractionComponent> NewlyHoveredInteractable = NewHoveredActor->FindComponentByClass<UInteractionComponent>();
	if(!NewlyHoveredInteractable.IsValid())
	{
		return false;
	}
	return IsValidInteractionComponent(NewlyHoveredInteractable);
}

bool ACommonPlayerController::IsValidInteractionComponent(const TWeakObjectPtr<UInteractionComponent> HitInteractionComponent) const
{

	if(!HitInteractionComponent.IsValid())
	{
		return false;
	}
	
	float DistanceToCheck = 0.f;
	switch (HitInteractionComponent->Affiliation)
	{
	case EAffiliation::Allies:
	case EAffiliation::Neutral:
		// For allies and neutral actors, switch outline
		return true;
	case EAffiliation::None:
		return false;
	case EAffiliation::InteractionActor:
		DistanceToCheck = INTERACT_OUTLINE_DISTANCE;
		break;
	case EAffiliation::Destructible:
	case EAffiliation::Enemies:
		// Proceed to see if enemies are in range to be outlined (aka in range to be fired at)
		DistanceToCheck = ValidAbilityOutlineDistance;
		break;
	default: ;
	}
	
	const AActor* InteractableOwner = HitInteractionComponent->GetOwner();
	if(InteractableOwner && PlayerCharacter.IsValid())
	{
		const FVector& InteractableLocation = InteractableOwner->GetActorLocation();
		const FVector& PlayerLocation = PlayerCharacter->GetActorLocation();
		const double DistanceToEnemy = UKismetMathLibrary::Vector_Distance(InteractableLocation, PlayerLocation);
		if(DistanceToEnemy <= DistanceToCheck)
		{
			return true;
		}			
	}	
	return false;
}

FHitResult ACommonPlayerController::Internal_ScanForTargetedActors() const
{
	FHitResult TempResult;
	switch (CameraType)
	{
	case ECameraType::FirstPerson:
	case ECameraType::ThirdPerson:
		Internal_GetShooterCameraTargetedActor(TempResult);
		break;
	case ECameraType::TopDown:
		GetHitResultUnderCursor(COMMON_TRACE_INTERACTION, false, TempResult);
		break;
	default: ;
	}
	return TempResult;
}

bool ACommonPlayerController::IsInRangeOfInteractable(const UInteractionComponent* InteractionComponent) const
{
	if (!PlayerCharacter.IsValid() || !InteractionComponent) {
		return false;
	}

	const float DistanceToProtagonist = FVector::Dist(InteractionComponent->GetOwnerLocation(), PlayerCharacter->GetActorLocation());
	switch (UCommonInteractUtils::GetAffiliationOfActor(InteractionComponent->GetOwner()))
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

