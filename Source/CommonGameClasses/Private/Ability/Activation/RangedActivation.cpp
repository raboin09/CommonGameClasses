#include "Ability/Activation/RangedActivation.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "ActorComponent/MountManagerComponent.h"
#include "ActorComponent/TopDownInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCoreTypes.h"
#include "Utils/CommonCombatUtils.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInputUtils.h"
#include "Utils/CommonInteractUtils.h"


void URangedActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	Fire(TriggerEventPayload);
	BPI_PlayFireFX(GetRaycastOriginLocation());
	AbilityActivationEvent.Broadcast({});
	BPI_OnActivation();
	if(bRotateCharacterToMouse)
	{
		if(UTopDownInputComponent* TopDownAimingComponent = GetInstigator()->FindComponentByClass<UTopDownInputComponent>())
		{
			TopDownAimingComponent->TryRotateActorToMouse();
		}
	}
}

void URangedActivation::Deactivate()
{
	AbilityDeactivationEvent.Broadcast({});
	BPI_OnDeactivation();
}

void URangedActivation::InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent)
{
	Super::InitActivationMechanism(OwnerMeshComponent);
	Internal_AssignOwningController();
}

void URangedActivation::PostInitProperties()
{
	Super::PostInitProperties();
	if (AimAssistObjectTypes.Num() == 0)
	{
		AimAssistObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	}

}

void URangedActivation::Internal_AssignOwningController()
{
	const TWeakObjectPtr<APawn> CurrentInstigator = GetInstigator();
	if(!CurrentInstigator.IsValid())
	{
		return;
	}

	if(APlayerController* TempPlayerController = Cast<APlayerController>(CurrentInstigator->GetController()))
	{
		OwningPlayerController = TempPlayerController;
	} else if(AAIController* TempAIController = Cast<AAIController>(CurrentInstigator->GetController()))
	{
		OwningAIController = TempAIController;
	}
}

void URangedActivation::Internal_GetTraceLocations(FVector& StartTrace, FVector& EndTrace)
{
	const FVector AimDirection = Internal_GetAimDirection(DefaultLineTraceDirection);
	StartTrace = Internal_GetStartTraceLocation(AimDirection);
	if(bHasFiringSpread)
	{
		EndTrace = StartTrace + Internal_GetFiringSpreadDirection(AimDirection) * TraceRange; 
	} else
	{
		EndTrace = StartTrace + AimDirection * TraceRange;
	}
}

FVector URangedActivation::Internal_GetStartTraceLocation(const FVector AimDirection) const
{
	switch (DefaultLineTraceDirection) {
		case ELineTraceDirection::Camera:
			return Internal_GetCameraStartLocation(AimDirection);
		default:
			return GetRaycastOriginLocation();
	}
}

FVector URangedActivation::Internal_GetCameraStartLocation(const FVector AimDirection) const
{
	if (!OwningPlayerController.IsValid())
	{
		return GetRaycastOriginLocation();
	}
	FRotator UnusedRot;
	FVector OutStartTrace;
	OwningPlayerController->GetPlayerViewPoint(OutStartTrace, UnusedRot);
	return OutStartTrace + AimDirection * ((GetInstigator()->GetActorLocation() - OutStartTrace) | AimDirection);
}

FVector URangedActivation::Internal_GetAimDirection(ELineTraceDirection LineTraceDirection) const
{
	FVector BaseAimDirection;
	switch (LineTraceDirection)
	{
		case ELineTraceDirection::Camera:
			// By default this is Camera for PlayerControllers, Controller rotation for AI
			BaseAimDirection = GetInstigator()->GetBaseAimRotation().Vector();
			break;
		case ELineTraceDirection::MeshSocket:
			// Use the socket rotation
			BaseAimDirection = GetRaycastOriginRotation();
			break;
		case ELineTraceDirection::Mouse:
			if(UCommonInputUtils::IsUsingGamepad(this))
			{
				BaseAimDirection = Internal_GetAimDirection(GamepadLineTraceDirection);
			} else
			{
				BaseAimDirection = Internal_GetMouseAim();	
			}
			break;
		case ELineTraceDirection::AbilityMeshForwardVector:
			BaseAimDirection = GetOwner()->GetActorForwardVector();
			break;
		case ELineTraceDirection::InstigatorForwardVector:
			BaseAimDirection = GetInstigator()->GetActorForwardVector();
			break;
		default:
			BaseAimDirection = FVector::ZeroVector;
			break;
	}

	if (bEnableAimAssist)
	{
		return TryGetAimAssistDirection(BaseAimDirection);
	}
	return BaseAimDirection;
}

FVector URangedActivation::TryGetAimAssistDirection(const FVector& OriginalAimDirection) const
{
	const FVector StartLocation = GetRaycastOriginLocation();
    
	// Find the best target for aim assist
	if (AActor* TargetActor = FindBestAimAssistTarget(StartLocation, OriginalAimDirection))
	{
		// Get target center location
		FVector TargetLocation = TargetActor->GetActorLocation();
        
		// If the target has a mesh, aim at its center
		if (USkeletalMeshComponent* TargetMesh = TargetActor->FindComponentByClass<USkeletalMeshComponent>())
		{
			TargetLocation = TargetMesh->GetComponentLocation() + 
				FVector(0, 0, TargetMesh->Bounds.BoxExtent.Z * 0.5f);
		}

		// Calculate directions
		FVector DirectionToTarget = (TargetLocation - StartLocation).GetSafeNormal();
        return FVector(DirectionToTarget.X, DirectionToTarget.Y, OriginalAimDirection.Z).GetSafeNormal();
	}

	return OriginalAimDirection;
}

AActor* URangedActivation::FindBestAimAssistTarget(const FVector& StartLocation, const FVector& AimDirection) const
{
    if (!GetInstigator())
    {
        return nullptr;
    }

    // Setup for overlap sphere
    TArray<AActor*> OverlappingActors;
    TArray<AActor*> ActorsToIgnore = GetActorsToIgnoreCollision();

    // Find all potential targets within range
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        StartLocation,
        AimAssistRange,
        AimAssistObjectTypes,
        nullptr,
        ActorsToIgnore,
        OverlappingActors
    );

    // Find the best target based on angle and distance
    AActor* BestTarget = nullptr;
    float BestScore = FLT_MAX;
    const float MaxCosAngle = FMath::Cos(FMath::DegreesToRadians(AimAssistAngle));

    for (AActor* PotentialTarget : OverlappingActors)
    {
        if (!IsValidAimAssistTarget(PotentialTarget))
        {
            continue;
        }

        FVector DirectionToTarget = (PotentialTarget->GetActorLocation() - StartLocation).GetSafeNormal();
        float CosAngle = FVector::DotProduct(AimDirection, DirectionToTarget);

        // Check if target is within aim assist angle
        if (CosAngle < MaxCosAngle)
        {
            continue;
        }

        // Check line of sight
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActors(ActorsToIgnore);

        if (GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLocation,
            PotentialTarget->GetActorLocation(),
            ECC_Visibility,
            QueryParams))
        {
            if (HitResult.GetActor() != PotentialTarget)
            {
                continue;
            }
        }

        // Calculate score (lower is better)
        float Distance = FVector::Distance(StartLocation, PotentialTarget->GetActorLocation());
        float AngleScore = (1.0f - CosAngle) * 1000.0f; // Prioritize targets closer to aim direction
        float Score = Distance + AngleScore;

        if (Score < BestScore)
        {
            BestScore = Score;
            BestTarget = PotentialTarget;
        }
    }

    return BestTarget;
}

bool URangedActivation::IsValidAimAssistTarget(AActor* Target) const
{
	if (!Target)
	{
		return false;
	}

	if(Target == GetInstigator())
	{
		return false;
	}

	EAffiliation Affiliation = UCommonInteractUtils::GetAffiliationRelatedToPlayerCharacter(Target);
	if(Affiliation != AimAssistAffiliation)
	{
		return false;
	}

	if(UGameplayTagComponent::ActorHasGameplayTag(Target, CommonStateTags::Dead))
	{
		return false;
	}
	return true;
}

FVector URangedActivation::Internal_GetFiringSpreadDirection(const FVector AimDirection)
{
	const int32 RandomSeed = FMath::Rand();
	const FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = TraceSpread + CurrentFiringSpread;
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	CurrentFiringSpread = FMath::Min(FiringSpreadMax, CurrentFiringSpread + FiringSpreadIncrement);
	return WeaponRandomStream.VRandCone(AimDirection, ConeHalfAngle, ConeHalfAngle);
}

FVector URangedActivation::Internal_GetMouseAim() const
{
	if(!OwningPlayerController.IsValid())
	{
		return FVector::ZeroVector;
	}
	FHitResult TempResult;
	OwningPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, TempResult);
	const FVector TempAimDir = TempResult.ImpactPoint - GetRaycastOriginLocation();
	const FVector SocketRot = GetRaycastOriginRotation();
	FVector AimDir = FVector(TempAimDir.X, TempAimDir.Y, SocketRot.Z);
	AimDir.Normalize();
	return AimDir;
}

FHitResult URangedActivation::AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact)
{
	if (Impact.bBlockingHit)
	{
		FHitResult UseImpact = Impact;
		if (!Impact.Component.IsValid())
		{
			const FVector StartTrace = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;
			const FVector EndTrace = Impact.ImpactPoint - Impact.ImpactNormal * 10.0f;
			FHitResult Hit = WeaponTrace(bShouldLineTrace, 5.f, StartTrace, EndTrace);
			UseImpact = Hit;
			return UseImpact;
		}
	}
	return Impact;
}

FVector URangedActivation::GetRaycastOriginRotation() const
{
	if(!MeshComponentRef.IsValid())
	{
		return FVector::ZeroVector;	
	}
	return MeshComponentRef->GetSocketRotation(MeshSocketName).Vector();
}

FVector URangedActivation::GetRaycastOriginLocation() const
{
	if(!MeshComponentRef.IsValid())
	{
		return FVector::ZeroVector;	
	}
	return MeshComponentRef->GetSocketLocation(MeshSocketName);
}

FHitResult URangedActivation::WeaponTrace(bool bLineTrace, float CircleRadius, FVector StartOverride, FVector EndOverride)
{
	FVector StartTrace, EndTrace;
	Internal_GetTraceLocations(StartTrace, EndTrace);
	StartTrace = StartOverride != FVector::ZeroVector ? StartOverride : StartTrace;
	EndTrace = EndOverride != FVector::ZeroVector ? EndOverride : EndTrace;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;
	FHitResult Hit(ForceInit);
	TArray<AActor*> IgnoreActors; 
	IgnoreActors.Append(GetActorsToIgnoreCollision());
	auto WeaponTraceType = UEngineTypes::ConvertToTraceType(COMMON_TRACE_ABILITY);
	auto DrawDebugTrace = bDrawDebugTrace ? EDrawDebugTrace::None : EDrawDebugTrace::ForDuration;
	if(bLineTrace)
	{
		UKismetSystemLibrary::LineTraceSingle(this, StartTrace, EndTrace, WeaponTraceType, false, IgnoreActors, DrawDebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f);
	} else
	{
		UKismetSystemLibrary::SphereTraceSingle(this, StartTrace, EndTrace, CircleRadius, WeaponTraceType, false, IgnoreActors, DrawDebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f);	
	}
	return Hit;
}

TArray<AActor*> URangedActivation::GetActorsToIgnoreCollision() const
{
	TArray<AActor*> IgnoredActors;
	// Ignore the Character/Pawn instigator AND the Ability owner
	IgnoredActors.Add(GetInstigator());
	IgnoredActors.Add(GetOwner());
	// If instigator is a Character, ignore their mount (if any)
	if(const UMountManagerComponent* MountManager = GetInstigator()->FindComponentByClass<UMountManagerComponent>())
	{
		if(AActor* CastedMount = Cast<AActor>(MountManager->GetCurrentMount().GetObject()))
		{
			IgnoredActors.AddUnique(CastedMount);
		}
	}
	return IgnoredActors;
}