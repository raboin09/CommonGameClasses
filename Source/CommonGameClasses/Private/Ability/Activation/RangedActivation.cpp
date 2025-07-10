﻿#include "Ability/Activation/RangedActivation.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "ActorComponent/MountManagerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCoreTypes.h"


void URangedActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	Fire(TriggerEventPayload);
	BPI_PlayFireFX(GetRaycastOriginLocation());
	AbilityActivationEvent.Broadcast({});
	BPI_OnActivation();
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
	const FVector AimDirection = Internal_GetAimDirection();
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
	switch (LineTraceDirection) {
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

FVector URangedActivation::Internal_GetAimDirection() const
{
	switch (LineTraceDirection)
	{
	case ELineTraceDirection::Camera:
		// By default this is Camera for PlayerControllers, Controller rotation for AI
		return GetInstigator()->GetBaseAimRotation().Vector();
	case ELineTraceDirection::MeshSocket:
		// Use the socket rotation
		return GetRaycastOriginRotation();
	case ELineTraceDirection::Mouse:
		return Internal_GetMouseAim();
	case ELineTraceDirection::AbilityMeshForwardVector:
		return GetOwner()->GetActorForwardVector();
	case ELineTraceDirection::InstigatorForwardVector:
		return GetInstigator()->GetActorForwardVector();
	default:
		return FVector::ZeroVector;
	}
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
	return FVector(TempAimDir.X, TempAimDir.Y, SocketRot.Z);
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
			FHitResult Hit = WeaponTrace(ShouldLineTrace(), 5.f, StartTrace, EndTrace);
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