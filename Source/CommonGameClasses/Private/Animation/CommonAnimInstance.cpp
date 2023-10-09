// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CommonAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Animation/CharacterAnimationComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCommonAnimInstance::UCommonAnimInstance()
{

}

void UCommonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	InitCharacterComponents();
}

void UCommonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	DeltaTick = DeltaSeconds;
	
	GetCharacterMovementData();
	GetMovementStates();
}

void UCommonAnimInstance::GetCharacterMovementData()
{	
	const APawn* PawnOwner = TryGetPawnOwner();
	if(!PawnOwner)
	{
		return;
	}
	
	Velocity = PawnOwner->GetVelocity().Size();
	const FVector& VelocityVector = PawnOwner->GetVelocity();
	const FRotator& Rotation = PawnOwner->GetActorRotation();

	UpVelocity = Rotation.UnrotateVector(VelocityVector).Z;
	ForwardVelocity = Rotation.UnrotateVector(VelocityVector).X;
	LeftVelocity = Rotation.UnrotateVector(VelocityVector).Y;
	bIsIdle = FMath::IsNearlyZero(FVector(VelocityVector.X, VelocityVector.Y, 0.0f).Size());
	
	if(!bIsIdle)
	{
		Direction = UKismetAnimationLibrary::CalculateDirection(VelocityVector, Rotation);
	}
	if(OwningMovementComponent)
	{
		InputAcceleration = OwningMovementComponent->GetCurrentAcceleration();
	}
}

void UCommonAnimInstance::GetMovementStates()
{
	if(!OwningMovementComponent)
	{
		return;
	}
	bIsFalling = OwningMovementComponent->IsFalling();
	bIsCrouching = OwningMovementComponent->IsCrouching();
	bIsFlying = OwningMovementComponent->IsFlying();
	bIsWalkingState = OwningMovementComponent->IsWalking();
	bIsGrounded = OwningMovementComponent->IsMovingOnGround();
	bIsInAir = bIsFalling || bIsFlying;
	MovementMode = OwningMovementComponent->MovementMode;
	bIsStanding = !bIsCrouching;
}

void UCommonAnimInstance::InitCharacterComponents()
{
	OwningCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if(OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
		CharacterAnimationComponent = OwningCharacter->FindComponentByClass<UCharacterAnimationComponent>();
	}
}

void UCommonAnimInstance::DisableRootMotionModeForDuration(float Duration)
{
	RootMotionCache = RootMotionMode;
	RootMotionMode = ERootMotionMode::IgnoreRootMotion;
	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle_RootMotionDisable, this, &UCommonAnimInstance::EnableRootMotionMode, Duration, false);	
	}
}

void UCommonAnimInstance::EnableRootMotionMode()
{
	RootMotionMode = RootMotionCache;
}