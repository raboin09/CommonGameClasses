#include "Ability/Activation/RangedActivation.h"

#include "AIController.h"
#include "Character/CommonCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/TagTypes.h"

ARangedActivation::ARangedActivation()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARangedActivation::Activate()
{
	Fire();
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
	Internal_AssignOwningMesh();
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

void ARangedActivation::Internal_AssignOwningMesh()
{	
	// Find the first non-null mesh component that we come across
	if(MeshType == EMeshType::InstigatorMesh)
	{
		// Check if Instigator is a char, if so then GetMesh(). Otherwise, find first skel mesh
		if(const ACharacter* CharRef = Cast<ACharacter>(GetInstigator()))
		{
			MeshComponentRef = CharRef->GetMesh();
		} else
		{
			MeshComponentRef = GetInstigator()->FindComponentByClass<USkeletalMeshComponent>();	
		}
	} else if (MeshType == EMeshType::AbilityMesh)
	{
		// Find the first SkeletalMesh, if it's nullptr then find the first StaticMesh
		MeshComponentRef = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		MeshComponentRef = MeshComponentRef ? MeshComponentRef : GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	}
}

bool ARangedActivation::Internal_ShouldEyeTrace() const
{
	return UGameplayTagComponent::ActorHasGameplayTag(GetInstigator(), TAG_STATE_AIMING) && bAimOriginIsPlayerEyesInsteadOfWeapon;
}

bool ARangedActivation::ShouldLineTrace() const
{
	return IsValid(OwningAIController);
}

FHitResult ARangedActivation::AdjustHitResultIfNoValidHitComponent(const FHitResult& Impact) const
{
	if (Impact.bBlockingHit)
	{
		FHitResult UseImpact = Impact;
		if (!Impact.Component.IsValid())
		{
			const FVector StartTrace = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;
			const FVector EndTrace = Impact.ImpactPoint - Impact.ImpactNormal * 10.0f;
			FHitResult Hit = WeaponTrace(StartTrace, EndTrace, ShouldLineTrace());
			UseImpact = Hit;
			return UseImpact;
		}
	}
	return Impact;
}

FVector ARangedActivation::GetShootDirection(const FVector& AimDirection)
{
	if(!bHasFiringSpread && IsPlayerControlled())
	{
		return AimDirection;
	}
	
	const int32 RandomSeed = FMath::Rand();
	const FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = TraceSpread + CurrentFiringSpread;
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	CurrentFiringSpread = FMath::Min(FiringSpreadMax, CurrentFiringSpread + FiringSpreadIncrement);
	return WeaponRandomStream.VRandCone(AimDirection, ConeHalfAngle, ConeHalfAngle);
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
			return InstigatorMesh->GetBoneLocation(MeshSocketName);
		}
		if (const UStaticMeshComponent* InstigatorMesh = GetInstigator()->FindComponentByClass<UStaticMeshComponent>())
		{
			return InstigatorMesh->GetSocketLocation(MeshSocketName);
		}
	}	
	return FVector::ZeroVector;
}

FVector ARangedActivation::GetAimDirection() const
{
	const APawn* CurrentInstigator = GetInstigator(); 
	if(!CurrentInstigator)
	{
		return FVector::ZeroVector;
	}

	if(const APlayerController* PlayerController = Cast<APlayerController>(CurrentInstigator->Controller))
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		return CamRot.Vector();
	}

	if(const AAIController* AIController = Cast<AAIController>(GetInstigator()->Controller))
	{
		return AIController->GetControlRotation().Vector();
	}
	return GetInstigator()->GetBaseAimRotation().Vector();
}

FVector ARangedActivation::GetCameraDamageStartLocation(const FVector& AimDirection) const
{
	FVector OutStartTrace = FVector::ZeroVector;	
	if(!Internal_ShouldEyeTrace())
	{
		OutStartTrace = GetRaycastOriginLocation();
	}
	else if (OwningPlayerController)
	{
		FRotator UnusedRot;
		OwningPlayerController->GetPlayerViewPoint(OutStartTrace, UnusedRot);
		OutStartTrace = OutStartTrace + AimDirection * ((GetInstigator()->GetActorLocation() - OutStartTrace) | AimDirection);
	}
	else if (OwningAIController)
	{
		OutStartTrace = GetRaycastOriginLocation();
	}
	return OutStartTrace;
}

FVector ARangedActivation::GetAdjustedAim() const
{
	if(OwningPlayerController)
	{
		FVector CamLoc;
		FRotator CamRot;
		OwningPlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		return CamRot.Vector();
	}	
	if(OwningAIController)
	{
		return OwningAIController->GetControlRotation().Vector();
	}
	return GetInstigator()->GetBaseAimRotation().Vector();
}

FHitResult ARangedActivation::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, bool bLineTrace, float CircleRadius) const
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;
	FHitResult Hit(ForceInit);
	TArray<AActor*> IgnoreActors; 
	IgnoreActors.Add(GetInstigator());
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

TArray<AActor*> ARangedActivation::GetActorsToIgnoreCollision() const
{
	TArray<AActor*> IgnoredActors;
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