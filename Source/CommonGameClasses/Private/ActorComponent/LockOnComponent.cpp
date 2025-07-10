// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/LockOnComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonTagTypes.h"
#include "Utils/CommonCoreUtils.h"
#include "Utils/CommonInteractUtils.h"

ULockOnComponent::ULockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SelectedActor = nullptr;
}

void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	LockOnInterpTimeline.SetTimelineLength(LockOnSlideDuration);
	LockOnInterpTimeline.SetTimelineLengthMode(TL_TimelineLength);
	LockOnInterpTimeline.SetLooping(false);
	
	FOnTimelineEvent InterpUpdateFunction;
	InterpUpdateFunction.BindDynamic(this, &ThisClass::Internal_InterpTransitionUpdate);
	LockOnInterpTimeline.SetTimelinePostUpdateFunc(InterpUpdateFunction);
	
	FOnTimelineEvent InterpFinishedFunction;
	InterpFinishedFunction.BindDynamic(this, &ThisClass::Internal_InterpTransitionFinished);
	LockOnInterpTimeline.SetTimelineFinishedFunc(InterpFinishedFunction);
	
	SetComponentTickEnabled(false);
}

void ULockOnComponent::InterpToBestTargetForMeleeAttack()
{
	const AActor* OwnerActor = GetOwner();
	if(!OwnerActor)
	{
		return;
	}

	TWeakObjectPtr<AActor> SelectedTarget;
	// If player controlled owner, interp to hovered actor if withiin valid distance
	if(UCommonCoreUtils::IsObjectPlayerControlled(OwnerActor))
	{
		SelectedTarget = UCommonCoreUtils::GetHoveredInteractionActorByPlayerController(this);
	}

	if(!Internal_IsActorValidTarget(SelectedTarget))
	{
		SelectedTarget = Internal_TraceForTarget();
		if(!SelectedTarget.IsValid())
		{
			return;
		}
	}
	
	UGameplayTagComponent::AddTagToActor(SelectedTarget.Get(), CommonGameState::CannotMove);
	InterpToActor(SelectedTarget);	
}

void ULockOnComponent::InterpToActor(TWeakObjectPtr<AActor> ActorToInterpTo)
{
	SetComponentTickEnabled(true);
	SelectedActor = ActorToInterpTo;
	if(!SelectedActor.IsValid())
	{
		return;
	}
	
	if(AActor* SourceActor = GetOwner(); bUseControllerRotation)
	{
		SourceActor->SetActorRotation(Internal_GetControllerAndActorBlendedRotation(SourceActor));
		const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(SourceActor->GetActorLocation(), SelectedActor->GetActorLocation());
		const FRotator SourceRot = SourceActor->GetActorRotation();
		TargetActorRotation.Pitch = SourceRot.Pitch;
		TargetActorRotation.Yaw = LookAt.Yaw;
		TargetActorRotation.Roll = SourceRot.Roll;
	}	
	TargetActorLocation = SelectedActor->GetActorLocation();
	Internal_StartInterpTransition();
}

void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(LockOnInterpTimeline.IsPlaying())
	{
		LockOnInterpTimeline.TickTimeline(DeltaTime);
	}
}

void ULockOnComponent::Internal_StartInterpTransition()
{
	if(!SelectedActor.IsValid())
	{
		return;
	}
	LockOnInterpTimeline.IsPlaying() ? LockOnInterpTimeline.Play() : LockOnInterpTimeline.PlayFromStart();
}

void ULockOnComponent::Internal_InterpTransitionUpdate()
{
	if(!SelectedActor.IsValid())
	{
		return;
	}
	const FTransform& SelectedActorTransform = GetOwner()->GetActorTransform();
	FTransform TargetTransform;
	TargetTransform.SetLocation(FVector(TargetActorLocation.X, TargetActorLocation.Y, SelectedActorTransform.GetLocation().Z));
	TargetTransform.SetRotation(FQuat(TargetActorRotation));
	const float Alpha = LockOnInterpTimeline.GetPlaybackPosition() / LockOnInterpTimeline.GetTimelineLength();
	const FTransform& NewActorTransform = UKismetMathLibrary::TLerp(SelectedActorTransform, TargetTransform, Alpha, ELerpInterpolationMode::QuatInterp);
	GetOwner()->SetActorLocationAndRotation(NewActorTransform.GetTranslation(), NewActorTransform.GetRotation(), true);
}

void ULockOnComponent::Internal_InterpTransitionFinished()
{
	UGameplayTagComponent::RemoveTagFromActor(SelectedActor.Get(), CommonGameState::CannotMove);
	SelectedActor.Reset();
	SetComponentTickEnabled(false);
}

TWeakObjectPtr<AActor> ULockOnComponent::Internal_TraceForTarget() const
{
	TWeakObjectPtr<AActor> SourceActor = GetOwner();
	if(!SourceActor.IsValid())
	{
		return nullptr;
	}
	
	TArray<FHitResult> TempHitResults;
	TArray<FHitResult> OutHitResults;
	float DegreeIncrements = ConeTraceArcWidth / UKismetMathLibrary::Max(1.f, NumCones);
	float YawValueFirst = ConeTraceArcWidth * -.5;
	TArray<AActor*> AlreadyHitActors;
	for (int i = 0; i < NumCones; i++) {
		float YawFinal = YawValueFirst + (DegreeIncrements * i);

		FVector StartTrace;
		FVector EndTrace;
		
		if(bUseControllerRotation)
		{
			StartTrace = SourceActor->GetActorLocation() + TraceOffset;
			FRotator FinalRot = Internal_GetControllerAndActorBlendedRotation(SourceActor);
			EndTrace = StartTrace + (FinalRot.Vector().RotateAngleAxis(YawFinal, FVector(0, 0, 1))) * MELEE_OUTLINE_DISTANCE;
		} else
		{
			FVector RotatedVector = SourceActor->GetActorForwardVector().RotateAngleAxis(YawFinal, FVector(0, 0, 1)) * MELEE_OUTLINE_DISTANCE;
			StartTrace = SourceActor->GetActorLocation() + TraceOffset;
			EndTrace = RotatedVector + StartTrace;
			StartTrace += TraceOffset;
		}

		TArray<TEnumAsByte <EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartTrace, EndTrace, SweepRadius, ObjectTypes, false, { SourceActor.Get() }, bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, TempHitResults, true);
		for (FHitResult HitResult : TempHitResults) {
			if(UCommonInteractUtils::AreActorsEnemies(SourceActor.Get(), HitResult.GetActor()))
			{
				OutHitResults.Add(HitResult);
			}
		}
	}
	return Internal_FindBestTargetFromActors(OutHitResults);
}

TWeakObjectPtr<AActor> ULockOnComponent::Internal_FindBestTargetFromActors(TArray<FHitResult> PotentialHitResults)
{
	if(PotentialHitResults.Num() <= 0)
	{
		return nullptr;
	}
	
	TArray<AActor*> HitActors;
	for(FHitResult TempHit : PotentialHitResults)
	{
		if(!UGameplayTagComponent::ActorHasGameplayTag(TempHit.GetActor(), CommonGameState::Dead))
		{
			HitActors.Add(TempHit.GetActor());
		}
	}
	return HitActors[0];
}

FRotator ULockOnComponent::Internal_GetControllerAndActorBlendedRotation(TWeakObjectPtr<AActor> SourceActor)
{
	const APlayerController* PlayerController = nullptr;
	if(const APawn* PawnObj = Cast<APawn>(SourceActor))
	{
		PlayerController = Cast<APlayerController>(PawnObj->Controller);
	}

	if(!PlayerController)
	{
		return SourceActor->GetActorRotation();
	}
	
	const FRotator StartRot = SourceActor->GetActorRotation();		
	FVector TempLoc;
	FRotator TempRot;
	PlayerController->GetPlayerViewPoint(TempLoc, TempRot);		
	return FRotator(StartRot.Pitch, TempRot.Yaw, StartRot.Roll);
}

bool ULockOnComponent::Internal_IsActorValidTarget(TWeakObjectPtr<AActor> InActor) const
{
	if(!InActor.IsValid())
	{
		return false;
	}

	const AActor* OwnerActor = GetOwner();
	if(!OwnerActor)
	{
		return false;
	}
	return UKismetMathLibrary::Vector_Distance(InActor->GetActorLocation(), OwnerActor->GetActorLocation()) > MELEE_OUTLINE_DISTANCE;
}
