// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/TopDownInputComponent.h"

#include "Character/CommonPlayerCharacter.h"
#include "Core/CommonCoreDeveloperSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

void UTopDownInputComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bShouldRotateCharacterToAiming)
	{
		return;
	}

	if (bUsingGamepad)
	{
		Internal_RotateCharacterToGamepad(true);
	} else {
		Internal_RotateCharacterToMouse(true);
	}
}

void UTopDownInputComponent::MoveInput(const FVector2D& InputVector)
{
	Internal_MoveInput(InputVector);
}

void UTopDownInputComponent::CalculateAimAngle(float AxisX, float AxisY)
{
	if(!bShouldRotateCharacterToAiming)
	{
		return;
	}
	AimAngle = FMath::RadiansToDegrees(FMath::Atan2(AxisY, -AxisX));
}

FRotator UTopDownInputComponent::GetMouseRotation() const
{
	if (!PlayerController.IsValid() || !PlayerCharacter.IsValid())
	{
		return FRotator::ZeroRotator;
	}

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


void UTopDownInputComponent::Internal_RotateCharacterToMouse(bool bInterpRotation)
{
	if(!PlayerController.IsValid() || !PlayerCharacter.IsValid())
	{
		return;
	}

	if(bUsingGamepad)
	{
		return;
	}
	
	FRotator TargetRotation = GetMouseRotation();
	if (bInterpRotation)
	{
		FRotator CurrentRotation = PlayerCharacter->GetActorRotation();
		FRotator NewRotation = CurrentRotation;
        
		// Calculate rotation delta
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);
        
		// Clamp rotation rate
		float DesiredRotationRate = DeltaYaw * RotationInterpSpeed;
		float ClampedRate = FMath::Clamp(DesiredRotationRate, -MaxRotationRate, MaxRotationRate);
        
		// Apply rotation
		if (FMath::Abs(DeltaYaw) > MinRotationRate)
		{
			NewRotation.Yaw += ClampedRate * UGameplayStatics::GetWorldDeltaSeconds(this);
			PlayerCharacter->SetActorRotation(NewRotation);
		}
	}
	else
	{
		PlayerCharacter->SetActorRotation(TargetRotation);
	}

}

void UTopDownInputComponent::Internal_RotateCharacterToGamepad(bool bInterpRotation)
{
	const FRotator OldRotation = PlayerCharacter->GetActorRotation();
	const FRotator TargetRot = FRotator(OldRotation.Pitch, -AimAngle, OldRotation.Roll);
	FRotator NewRotation = TargetRot;
	if(bInterpRotation)
	{
		NewRotation = FMath::RInterpTo(OldRotation, TargetRot, UGameplayStatics::GetWorldDeltaSeconds(this), 5.f);	
	}
	PlayerCharacter->SetActorRotation(NewRotation);
}

void UTopDownInputComponent::TryToggleUsingGamepad(bool bInUsingGamepad)
{
	if(bUsingGamepad == bInUsingGamepad)
	{
		return;
	}
	
	bUsingGamepad = bInUsingGamepad;
	if (!PlayerController.IsValid())
	{
		return;
	}
	PlayerController->SetShowMouseCursor(!bUsingGamepad);
	GamepadInputChanged.Broadcast(bUsingGamepad);
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

	TryToggleUsingGamepad(true);
	if(!bShouldRotateCharacterToAiming)
	{
		return;
	}
	LastAimInput = FVector2D(AxisX, AxisY);
	CalculateAimAngle(AxisX, AxisY);
}

void UTopDownInputComponent::MouseAimInput(const FVector2D& InputVector)
{
	TryToggleUsingGamepad(false);
	if(!PlayerController.IsValid())
	{
		return;
	}
	LastAimInput = InputVector;
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
	TryToggleUsingGamepad(bIsUsingGamepad);
	LastMoveInput = FVector2D(AxisX, AxisY);
	PlayerCharacter->MoveInput(AxisX, AxisY);
}

void UTopDownInputComponent::ToggleUseTwinStickAiming(bool bShouldUseTwinStickAiming)
{
	bShouldRotateCharacterToAiming = bShouldUseTwinStickAiming;
    
	if (UCharacterMovementComponent* CharacterMovementComponent = 
		GetOwner()->FindComponentByClass<UCharacterMovementComponent>())
	{
		CharacterMovementComponent->bOrientRotationToMovement = false;
	}
    
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		Pawn->bUseControllerRotationYaw = false;
	}
    
	if (USpringArmComponent* SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>())
	{
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bInheritRoll = false;
	}
}

void UTopDownInputComponent::SnapRotateActorToAim()
{
	if(bUsingGamepad)
	{

	} else
	{
		Internal_RotateCharacterToMouse(false);
	}
}

void UTopDownInputComponent::ToggleMovementOrientRotation(bool bOrientRotationToMovement)
{
	if(bUsingGamepad)
	{
		return;
	}
	
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
