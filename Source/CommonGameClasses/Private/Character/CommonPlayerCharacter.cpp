// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonPlayerCharacter.h"

#include "Player/CommonPlayerController.h"
#include "Utils/CommonCoreUtils.h"

void ACommonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(ACommonPlayerController* PlayerController = UCommonCoreUtils::GetCommonPlayerController(this))
	{
		PlayerController->OnCameraTypeChanged().AddUniqueDynamic(this, &ThisClass::HandleCameraTypeChanged);
	}
}

void ACommonPlayerCharacter::MoveInput(float Right, float Forward)
{
	LastMoveInput.X = Right;
	LastMoveInput.Y = Forward;
	
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardMoveDirection = FVector::ZeroVector;
	FVector RightMoveDirection = FVector::ZeroVector;
	
	switch(UCommonCoreUtils::GetCurrentCameraType(this))
	{
		case ECameraType::FirstPerson:
		case ECameraType::ThirdPerson:
			const FRotator YawRotation(0, ControlRotation.Yaw, 0);
			RightMoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			ForwardMoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			break;
		case ECameraType::TopDown:
			ForwardMoveDirection = ControlRotation.RotateVector(FVector::ForwardVector);
			RightMoveDirection = ControlRotation.RotateVector(FVector::RightVector);
			break;
		default: ;
	}
	AddMovementInput(ForwardMoveDirection, Forward);
	AddMovementInput(RightMoveDirection, Right);
	BPI_MoveInput(Right, Forward);
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
