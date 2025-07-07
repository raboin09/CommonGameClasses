// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/CommonPlayerController.h"
#include "Utils/CommonCoreUtils.h"

ACommonPlayerCharacter::ACommonPlayerCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	LastMoveInput = FVector2D::ZeroVector;
}

void ACommonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(ACommonPlayerController* PlayerController = UCommonCoreUtils::GetCommonPlayerController(this))
	{
		PlayerController->OnCameraTypeChanged().AddUniqueDynamic(this, &ThisClass::HandleCameraTypeChanged);
	}
}

void ACommonPlayerCharacter::MoveInput(float AxisX, float AxisY)
{
	LastMoveInput.X = AxisX;
	LastMoveInput.Y = AxisY;
	
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

void ACommonPlayerCharacter::Internal_GetMoveDirections(FVector& ForwardMoveDirection, FVector& RightMoveDirection) const
{
	const FRotator& ControlRotation = GetControlRotation();
	switch(UCommonCoreUtils::GetCurrentCameraType(this))
	{
		case ECameraType::FirstPerson:
		case ECameraType::ThirdPerson:
			const FRotator YawRotation(0, ControlRotation.Yaw, 0);
			RightMoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			ForwardMoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			break; 
		case ECameraType::TopDown:
			if(UCameraComponent* Camera = FindComponentByClass<UCameraComponent>())
			{
				FRotator CameraRotation = Camera->GetComponentRotation();
				CameraRotation.Pitch = 0;
				
				ForwardMoveDirection = UKismetMathLibrary::GetForwardVector(CameraRotation);
				RightMoveDirection = UKismetMathLibrary::GetRightVector(CameraRotation);
			}
			break;
		default: ;
	}
}
