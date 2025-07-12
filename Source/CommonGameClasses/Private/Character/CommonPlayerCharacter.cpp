// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/CommonPlayerController.h"
#include "Utils/CommonCoreUtils.h"

ACommonPlayerCharacter::ACommonPlayerCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	DefaultGameplayTags.Add(CommonCoreTags::PlayerCharacter);
}

void ACommonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(ACommonPlayerController* PlayerController = UCommonCoreUtils::GetCommonPlayerController(this))
	{
		PlayerController->OnCameraTypeChanged().AddUniqueDynamic(this, &ThisClass::HandleCameraTypeChanged);
	}
}

void ACommonPlayerCharacter::UnPossessed()
{
	if(ACommonPlayerController* PlayerController = UCommonCoreUtils::GetCommonPlayerController(this))
	{
		PlayerController->OnCameraTypeChanged().RemoveDynamic(this, &ThisClass::HandleCameraTypeChanged);
	}
	Super::UnPossessed();
}

void ACommonPlayerCharacter::MoveInput(float AxisX, float AxisY)
{	
	FVector ForwardMoveDirection = FVector::ZeroVector;
	FVector RightMoveDirection = FVector::ZeroVector;
	Internal_GetMoveDirections(ForwardMoveDirection, RightMoveDirection);
	
	AddMovementInput(ForwardMoveDirection, AxisX);
	AddMovementInput(RightMoveDirection, AxisY);
	BPI_MoveInput(AxisX, AxisY);
}

void ACommonPlayerCharacter::MouseInput(float Yaw, float Pitch)
{
	switch(UCommonCoreUtils::GetCurrentCameraType(this))
	{
		case ECameraType::FirstPerson:
		case ECameraType::ThirdPerson:
			AddControllerYawInput(Yaw);
			AddControllerPitchInput(Pitch);
		case ECameraType::TopDown:
			break;
		default: ;
	}
	BPI_MouseInput(Yaw, Pitch);
}

void ACommonPlayerCharacter::GetCardinalDirections(FVector& OutForwardDirection, FVector& OutRightDirection) const
{
	Internal_GetMoveDirections(OutForwardDirection, OutRightDirection);
}

void ACommonPlayerCharacter::HandleCameraTypeChanged(const FCameraTypeChangedPayload& CameraTypeChangedPayload)
{
	switch(CameraTypeChangedPayload.NewCameraType) {
		case ECameraType::FirstPerson:
			SetupFirstPersonCamera();
			break;
		case ECameraType::ThirdPerson:
			SetupThirdPersonCamera();
			break;
		case ECameraType::TopDown:
			SetupTopDownCamera();
			break;
		default: ;
	}
	BPI_CameraChanged(CameraTypeChangedPayload);
}

void ACommonPlayerCharacter::SetupThirdPersonCamera()
{
	BPI_SetupThirdPersonCamera();
}

void ACommonPlayerCharacter::SetupFirstPersonCamera()
{
	BPI_SetupFirstPersonCamera();
}

void ACommonPlayerCharacter::SetupTopDownCamera()
{
	BPI_SetupTopDownCamera();
}

void ACommonPlayerCharacter::Internal_GetMoveDirections(FVector& OutForwardMoveDirection, FVector& OutRightMoveDirection) const
{
	const FRotator& ControlRotation = GetControlRotation();
	switch(UCommonCoreUtils::GetCurrentCameraType(this))
	{
		case ECameraType::FirstPerson:
		case ECameraType::ThirdPerson:
			const FRotator YawRotation(0, ControlRotation.Yaw, 0);
			OutRightMoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			OutForwardMoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			break;
		case ECameraType::TopDown:
			if(APlayerController* TempController = Cast<APlayerController>(GetController()))
			{
				FRotator CameraRotation;
				FVector UnusedLocation;
				TempController->GetPlayerViewPoint(UnusedLocation, CameraRotation);
				CameraRotation.Pitch = 0;					
				OutForwardMoveDirection = UKismetMathLibrary::GetForwardVector(CameraRotation);
				OutRightMoveDirection = UKismetMathLibrary::GetRightVector(CameraRotation);	
			}
			break ;
		default: ;
	}
}
