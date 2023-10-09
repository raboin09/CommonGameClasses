// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ThirdPersonAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

UThirdPersonAnimInstance::UThirdPersonAnimInstance()
{
	TargetFrameRate = 60.f;
	LeanSmooth = 6.f;
	AimSmooth = 6.f;
}

void UThirdPersonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	GetLeaningData();
	GetAimingData();
}

void UThirdPersonAnimInstance::GetAimingData()
{
	const APawn* PawnOwner = TryGetPawnOwner();
	if(!PawnOwner)
	{
		return;
	}

	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(RawAimOffset, PawnOwner->GetActorRotation());
	const float Min = AimClamp * -1.0f;
	const float Max = AimClamp;

	float TargetX = Delta.Yaw;
	const float TargetY = Delta.Pitch;

	if(!UKismetMathLibrary::InRange_FloatFloat(Delta.Yaw, Min, Max, true, true))
	{
		switch(AimOffsetBehavior)
		{
		case EAimOffsetClamp::Nearest:
			TargetX = FMath::Clamp(Delta.Yaw, Min, Max);
			break;

		case EAimOffsetClamp::Left:
			TargetX = Min;
			break;

		case EAimOffsetClamp::Right:
			TargetX = Max;
			break;

		default:  
			checkNoEntry();
		}
	}

	FinalAimOffset = FMath::Vector2DInterpTo(FinalAimOffset, FVector2D(TargetX, TargetY),DeltaTick, AimSmooth);

	Delta = UKismetMathLibrary::NormalizedDeltaRotator(PreviousFrameAim, RawAimOffset);
	AimDelta = FMath::Vector2DInterpTo(AimDelta, FVector2D(NormalizeLean(Delta.Yaw), NormalizeLean(Delta.Pitch)), DeltaTick, LeanSmooth);
	PreviousFrameAim = RawAimOffset;
}

void UThirdPersonAnimInstance::GetLeaningData()
{
	const APawn* PawnOwner = TryGetPawnOwner();
	if(!PawnOwner)
	{
		return;
	}

	const FRotator& Rotation = PawnOwner->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(PreviousRotation, Rotation);
	Lean = FMath::FInterpTo(Lean, NormalizeLean(Delta.Yaw),DeltaTick,LeanSmooth);
	PreviousRotation = Rotation;
}