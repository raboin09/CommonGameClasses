﻿#include "Ability/Activation/RangedActivation.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "Character/CommonCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ARangedActivation::ARangedActivation()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARangedActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	Fire(TriggerEventPayload.ActivationLevel);
	K2_PlayFireFX(GetRaycastOriginLocation());
	AbilityActivationEvent.Broadcast({});
}

void ARangedActivation::Deactivate()
{
	AbilityDeactivationEvent.Broadcast({});
}

void ARangedActivation::BeginPlay()
{
	Super::BeginPlay();
	Internal_AssignOwningController();
}

void ARangedActivation::Internal_AssignOwningController()
{
	const APawn* CurrentInsigator = GetInstigator();
	if(!CurrentInsigator)
	{
		return;
	}

	if(const APlayerController* TempPlayerController = Cast<APlayerController>(CurrentInsigator->GetController()))
	{
		OwningPlayerController = TempPlayerController;
	} else if(const AAIController* TempAIController = Cast<AAIController>(CurrentInsigator->GetController()))
	{
		OwningAIController = TempAIController;
	}
}

void ARangedActivation::Internal_GetTraceLocations(FVector& StartTrace, FVector& EndTrace)
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

FVector ARangedActivation::Internal_GetStartTraceLocation(const FVector AimDirection) const
{
	switch (LineTraceDirection) {
		case ELineTraceDirection::Camera:
			return Internal_GetCameraStartLocation(AimDirection);
		case ELineTraceDirection::Mesh:
		case ELineTraceDirection::Mouse:
		default:
			return GetRaycastOriginLocation();
	}
}

FVector ARangedActivation::Internal_GetCameraStartLocation(const FVector AimDirection) const
{
	if (!OwningPlayerController)
	{
		return GetRaycastOriginLocation();
	}
	FRotator UnusedRot;
	FVector OutStartTrace;
	OwningPlayerController->GetPlayerViewPoint(OutStartTrace, UnusedRot);
	return OutStartTrace + AimDirection * ((GetInstigator()->GetActorLocation() - OutStartTrace) | AimDirection);
}

FVector ARangedActivation::Internal_GetAimDirection() const
{
	switch (LineTraceDirection)
	{
	case ELineTraceDirection::Camera:
		// By default this is Camera for PlayerControllers, Controller rotation for AI
		return GetInstigator()->GetBaseAimRotation().Vector();
	case ELineTraceDirection::Mesh:
		// Use the socket rotation
		return GetRaycastOriginRotation();
	case ELineTraceDirection::Mouse:
		return Internal_GetMouseAim();
	default:
		return FVector::ZeroVector;
	}
}

FVector ARangedActivation::Internal_GetFiringSpreadDirection(const FVector AimDirection)
{
	const int32 RandomSeed = FMath::Rand();
	const FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = TraceSpread + CurrentFiringSpread;
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	CurrentFiringSpread = FMath::Min(FiringSpreadMax, CurrentFiringSpread + FiringSpreadIncrement);
	return WeaponRandomStream.VRandCone(AimDirection, ConeHalfAngle, ConeHalfAngle);
}

FVector ARangedActivation::Internal_GetMouseAim() const
{
	if(!OwningPlayerController)
	{
		return FVector::ZeroVector;
	}
	FHitResult TempResult;
	OwningPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, TempResult);
	const FVector TempAimDir = TempResult.ImpactPoint - GetRaycastOriginLocation();
	const FVector SocketRot = GetRaycastOriginRotation();
	return FVector(TempAimDir.X, TempAimDir.Y, SocketRot.Z);
}

FHitResult ARangedActivation::AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact)
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

FVector ARangedActivation::GetRaycastOriginRotation() const
{
	if (MeshType == EMeshType::AbilityMesh)
	{
		if(!MeshComponentRef)
			return FVector::ZeroVector;
		return MeshComponentRef->GetSocketRotation(MeshSocketName).Vector();
	}
	
	if(MeshType == EMeshType::InstigatorMesh)
	{
		// Could be reduced to just UMeshComponent, but it's better to check Skeletal meshes first for Characters
		if(const USkeletalMeshComponent* InstigatorMesh = GetInstigator()->FindComponentByClass<USkeletalMeshComponent>())
		{
			return InstigatorMesh->GetSocketRotation(MeshSocketName).Vector();
		}
		if (const UStaticMeshComponent* InstigatorMesh = GetInstigator()->FindComponentByClass<UStaticMeshComponent>())
		{
			return InstigatorMesh->GetSocketRotation(MeshSocketName).Vector();
		}
	}	
	return FVector::ZeroVector;
}

FVector ARangedActivation::GetRaycastOriginLocation() const
{
	if (MeshType == EMeshType::AbilityMesh)
	{
		if(!MeshComponentRef)
			return FVector::ZeroVector;
		return MeshComponentRef->GetSocketLocation(MeshSocketName);
	}

	if(MeshType == EMeshType::InstigatorMesh)
	{
		if(const USkeletalMeshComponent* InstigatorMesh = GetInstigator()->FindComponentByClass<USkeletalMeshComponent>())
		{
			return InstigatorMesh->GetSocketLocation(MeshSocketName);
		}
		if (const UStaticMeshComponent* InstigatorMesh = GetInstigator()->FindComponentByClass<UStaticMeshComponent>())
		{
			return InstigatorMesh->GetSocketLocation(MeshSocketName);
		}
	}	
	return FVector::ZeroVector;
}

FHitResult ARangedActivation::WeaponTrace(bool bLineTrace, float CircleRadius, FVector StartOverride, FVector EndOverride)
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
	auto DrawDebugTrace = EDrawDebugTrace::None;
	auto WeaponTraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	if(bLineTrace)
	{
		UKismetSystemLibrary::LineTraceSingle(this, StartTrace, EndTrace, WeaponTraceType, false, IgnoreActors, DrawDebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 10.f);
	} else
	{
		UKismetSystemLibrary::SphereTraceSingle(this, StartTrace, EndTrace, CircleRadius, WeaponTraceType, false, IgnoreActors, DrawDebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 10.f);	
	}
	return Hit;
}

TArray<AActor*> ARangedActivation::GetActorsToIgnoreCollision()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetInstigator());
	IgnoredActors.Add(this);
	// If instigator is a Character, ignore their mount (if any)
	if(const ACommonCharacter* CastedChar = Cast<ACommonCharacter>(GetInstigator()))
	{
		if(AActor* CastedMount = Cast<AActor>(CastedChar->GetCurrentMount().GetObject()))
		{
			IgnoredActors.AddUnique(CastedMount);
		}
	}
	return IgnoredActors;
}