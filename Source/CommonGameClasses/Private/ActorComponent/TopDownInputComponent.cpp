// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/TopDownInputComponent.h"

#include "Character/CommonPlayerCharacter.h"
#include "Core/CommonCoreDeveloperSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/CommonCoreUtils.h"
#include "Utils/CommonInputUtils.h"

UTopDownInputComponent::UTopDownInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UTopDownInputComponent::BeginPlay()
{
	Super::BeginPlay();
	if(const UCommonCoreDeveloperSettings* GameSettings = GetDefault<UCommonCoreDeveloperSettings>())
	{
		CachedGamepadDeadZone = GameSettings->GamePadDeadZone;
		CachedGamepadSensitivity = GameSettings->GamePadSensitivity;	
	}
	
	PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerCharacter = UCommonCoreUtils::GetCommonPlayerCharacter(GetOwner());
}

void UTopDownInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(UCommonInputUtils::IsUsingGamepad(this) != bWasUsingGamepad)
	{
		bWasUsingGamepad = UCommonInputUtils::IsUsingGamepad(this);
		GamepadInputChanged.Broadcast(bWasUsingGamepad);
	}	
}

void UTopDownInputComponent::MoveInput(const FVector2D& InputVector)
{
	// Set this for in-place aiming mode, even though this may be overwritten in the other functions below
	UCommonInputUtils::SetLastMovementInput(InputVector);
	if(bGamepadRootedAimingMode && UCommonInputUtils::IsUsingGamepad(this))
	{
		GamepadAimInput(InputVector);
	} else
	{
		if(UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonStateTags::CannotMove))
		{
			return;
		}
		Internal_MoveInput(InputVector);
	}
}

FRotator UTopDownInputComponent::GetMouseRotation() const
{
	float MouseX, MouseY;
	if (!PlayerController->GetMousePosition(MouseX, MouseY))
	{
		return FRotator::ZeroRotator;
	}

	FVector WorldLocation, WorldDirection;
	if (PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
	{
		FVector ActorLocation = PlayerCharacter->GetActorLocation();
		FPlane GroundPlane(ActorLocation, FVector::UpVector);

		FVector IntersectionPoint = FMath::RayPlaneIntersection(
			WorldLocation,
			WorldLocation + WorldDirection * 1000000.0f,
			GroundPlane);
		FVector DirectionToMouse = (IntersectionPoint - ActorLocation).GetSafeNormal2D();
		return DirectionToMouse.Rotation();
	}
	return FRotator::ZeroRotator;
}

void UTopDownInputComponent::Internal_RotateCharacterToMouse(bool bInterpRotation) const
{
	if(UCommonInputUtils::IsUsingGamepad(this))
	{
		return;
	}
	Internal_RotateCharacter(GetMouseRotation(), bInterpRotation);
}

void UTopDownInputComponent::Internal_RotateCharacterToGamepad(bool bInterpRotation) const
{
	if(!UCommonInputUtils::IsUsingGamepad(this))
	{
		return;
	}
	FVector CardinalForward;
	FVector CardinalRight;
	PlayerCharacter->GetCardinalDirections(CardinalForward, CardinalRight);
	FRotator CurrentRotation = CardinalForward.Rotation();
	float AimAngle = FMath::RadiansToDegrees(FMath::Atan2(UCommonInputUtils::GetLastMovementInput().Y, UCommonInputUtils::GetLastMovementInput().X));
	const FRotator TargetRotation = FRotator(CurrentRotation.Pitch, AimAngle, CurrentRotation.Roll);
	Internal_RotateCharacter(TargetRotation, bInterpRotation);
}

void UTopDownInputComponent::Internal_RotateCharacter(const FRotator& TargetRotation, bool bShouldInterpRotation) const
{
	if(!PlayerCharacter.IsValid())
	{
		return;
	}
	
	if(bShouldInterpRotation)
	{
		FRotator CurrentRotation = PlayerCharacter->GetActorRotation();
		FRotator NewRotation = CurrentRotation;
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);
		float DesiredRotationRate = DeltaYaw * RotationInterpSpeed;
		float ClampedRate = FMath::Clamp(DesiredRotationRate, -MaxRotationRate, MaxRotationRate);
		
		if (FMath::Abs(DeltaYaw) > MinRotationRate)
		{
			NewRotation.Yaw += ClampedRate * UGameplayStatics::GetWorldDeltaSeconds(this);
			PlayerCharacter->SetActorRotation(NewRotation);
		}
	} else
	{
		PlayerCharacter->SetActorRotation(TargetRotation);
	}
}

void UTopDownInputComponent::GamepadAimInput(const FVector2D& InputVector)
{	
	float AxisX = InputVector.X;
	float AxisY = InputVector.Y;
	
	const bool bHasValidInputs = Internal_AdjustGamepadInputs(AxisX, AxisY);
	if(!bHasValidInputs)
	{
		return;
	}
	
	if(!bGamepadRootedAimingMode)
	{
		return;
	}
	Internal_RotateCharacterToGamepad(true);
}

void UTopDownInputComponent::MouseAimInput(const FVector2D& InputVector)
{
	if(!PlayerController.IsValid())
	{
		return;
	}
	UCommonInputUtils::SetLastMovementInput(InputVector);
	PlayerCharacter->MouseInput(InputVector.X, InputVector.Y);
}

void UTopDownInputComponent::Internal_MoveInput(const FVector2D& InputVector)
{
	float AxisX = InputVector.X;
	float AxisY = InputVector.Y;
	
	bool bIsUsingGamepad = UCommonInputUtils::IsUsingGamepad(this);
	bool bHasValidInputs;
	if(bIsUsingGamepad)
	{
		bHasValidInputs = Internal_AdjustGamepadInputs(AxisX, AxisY);	
	} else
	{
		if(FMath::Abs(AxisX) < .1)
		{
			AxisX = 0;
		}
	
		if(FMath::Abs(AxisY) < .1)
		{
			AxisY = 0;
		}
		bHasValidInputs = AreInputsValid(AxisX, AxisY);;
	}
	
	if(!bHasValidInputs)
	{
		return;
	}
	UCommonInputUtils::SetLastMovementInput(FVector2D(AxisX, AxisY));
	PlayerCharacter->MoveInput(AxisX, AxisY);
}

void UTopDownInputComponent::ToggleGamepadRootedAimingMode(bool bInGamepadRootedAimingMode)
{
	bGamepadRootedAimingMode = bInGamepadRootedAimingMode;
}

void UTopDownInputComponent::ToggleMovementOrientRotation(bool bOrientRotationToMovement)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>())
	{
		CharacterMovementComponent->bOrientRotationToMovement = bOrientRotationToMovement;
	}
}

bool UTopDownInputComponent::Internal_AdjustGamepadInputs(float& OutAxisX, float& OutAxisY) const
{
	Internal_TryAssignDeadzoneInput(OutAxisX, OutAxisY);
	Internal_AdjustInputSensitivity(OutAxisX, OutAxisY);
	return AreInputsValid(OutAxisX, OutAxisY);
}

void UTopDownInputComponent::Internal_TryAssignDeadzoneInput(float& OutAxisX, float& OutAxisY) const
{
	if(FMath::Abs(OutAxisX) < CachedGamepadDeadZone)
	{
		OutAxisX = 0;
	}
	
	if(FMath::Abs(OutAxisY) < CachedGamepadDeadZone)
	{
		OutAxisY = 0;
	}
}

void UTopDownInputComponent::Internal_AdjustInputSensitivity(float& OutAxisX, float& OutAxisY) const
{
	OutAxisX *= CachedGamepadSensitivity;
	OutAxisY *= CachedGamepadSensitivity;
}

bool UTopDownInputComponent::AreInputsValid(const float AxisX, const float AxisY)
{
	if(AxisX == 0 && AxisY == 0)
	{
		return false;
	}
	return true;
}
