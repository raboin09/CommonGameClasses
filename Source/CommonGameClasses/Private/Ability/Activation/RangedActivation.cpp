#include "Ability/Activation/RangedActivation.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "ActorComponent/MountManagerComponent.h"
#include "ActorComponent/TopDownInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/CommonInputUtils.h"
#include "Utils/CommonInteractUtils.h"


void URangedActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{	
	Super::Activate(TriggerEventPayload);
	Fire(TriggerEventPayload);
	BPI_PlayFireFX(GetRaycastOriginLocation());
	AbilityActivationEvent.Broadcast({});
	BPI_OnActivation();
}

void URangedActivation::Deactivate()
{
	Super::Deactivate();
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
	if (TraceForObjectTypes.Num() == 0)
	{
		TraceForObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		TraceForObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		TraceForObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
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

	if(bSnapCharacterRotationToEndTrace)
	{
		FVector DirectionToTarget = (EndTrace - GetInstigator()->GetActorLocation()).GetSafeNormal2D();
		FRotator NewRotation = DirectionToTarget.Rotation();
		GetInstigator()->SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
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
        TraceForObjectTypes,
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
			TArray<FHitResult> Hits = WeaponTrace(bShouldLineTrace, 5.f, StartTrace, EndTrace);
			for(auto Hit : Hits)
			{
				UseImpact = Hit;	
			}
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

TArray<FHitResult> URangedActivation::RemoveDuplicateHitResults(const TArray<FHitResult>& HitResults)
{
	TArray<FHitResult> UniqueHits;
	TSet<AActor*> ProcessedActors;
    
	for (const FHitResult& Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			// If we haven't processed this actor yet
			if (!ProcessedActors.Contains(HitActor))
			{
				ProcessedActors.Add(HitActor);
				UniqueHits.Add(Hit);
			}
		}
		else
		{
			// Always keep hits that don't have an actor (like landscape)
			UniqueHits.Add(Hit);
		}
	}
	return UniqueHits;
}


TArray<FHitResult> URangedActivation::WeaponTrace(bool bLineTrace, float CircleRadius, FVector StartOverride, FVector EndOverride)
{
	FVector StartTrace, EndTrace;
	Internal_GetTraceLocations(StartTrace, EndTrace);
	StartTrace = StartOverride != FVector::ZeroVector ? StartOverride : StartTrace;
	EndTrace = EndOverride != FVector::ZeroVector ? EndOverride : EndTrace;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;
	TArray<AActor*> IgnoreActors; 
	IgnoreActors.Append(GetActorsToIgnoreCollision());
	auto DrawDebugTrace = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<FHitResult> Hits;
	if(bShouldStopTraceAfterFirstSuccessfulHit)
	{
		FHitResult Hit(ForceInit);
		if(bLineTrace)
		{
			UKismetSystemLibrary::LineTraceSingleForObjects(this, StartTrace, EndTrace, TraceForObjectTypes, false, IgnoreActors, DrawDebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, DebugTraceDuration);
		} else
		{
			UKismetSystemLibrary::SphereTraceSingleForObjects(this, StartTrace, EndTrace, CircleRadius, TraceForObjectTypes, false, IgnoreActors, DrawDebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, DebugTraceDuration);	
		}
		Hits.Add(Hit);
	} else
	{
		if(bLineTrace)
		{
			UKismetSystemLibrary::LineTraceMultiForObjects(this, StartTrace, EndTrace, TraceForObjectTypes, false, IgnoreActors, DrawDebugTrace, Hits, true, FLinearColor::Red, FLinearColor::Green, DebugTraceDuration);
		} else
		{
			UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartTrace, EndTrace, CircleRadius, TraceForObjectTypes, false, IgnoreActors, DrawDebugTrace, Hits, true, FLinearColor::Red, FLinearColor::Green, DebugTraceDuration);
		}

		TArray<FHitResult> TempHits = Hits;
		Hits.Empty();
		bShouldStopTraceAfterFirstSuccessfulHit = true;
		for(const FHitResult& Hit : TempHits)
		{
			FHitResult TempHit = WeaponTrace(true)[0];
			if(TempHit.bBlockingHit && TempHit.GetActor() == Hit.GetActor())
			{
				Hits.Add(Hit);
			}
		}
		bShouldStopTraceAfterFirstSuccessfulHit = false;
	}
	return RemoveDuplicateHitResults(Hits);
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