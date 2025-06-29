// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CommonPlayerController.h"
#include "Utils/CommonCoreUtils.h"

ACommonPlayerCharacter::ACommonPlayerCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	FCameraTypeSettings ThirdPersonCameraSettings;
	ThirdPersonCameraSettings.TargetArmLength = 300.f;
	ThirdPersonCameraSettings.TargetArmOffset = FVector(0.f, 0.f, 75.f);
	ThirdPersonCameraSettings.bSpringArmUsePawnControlRotation = true;
	ThirdPersonCameraSettings.bEnableCameraLag = true;
	ThirdPersonCameraSettings.CameraLagSpeed = 10.f;
	ThirdPersonCameraSettings.bEnableCameraRotationLag = true;
	ThirdPersonCameraSettings.bCameraUsePawnControlRotation = true;
	ThirdPersonCameraSettings.bUseControllerRotationYaw = true;
	ThirdPersonCameraSettings.FieldOfView = 90.f;
	CameraTypeSettings.Add(ECameraType::ThirdPerson, ThirdPersonCameraSettings);

	FCameraTypeSettings TopDownCameraSettings;
	TopDownCameraSettings.TargetArmLength = 2200.f;
	TopDownCameraSettings.TargetArmRotation = FRotator(-50.f, 45.f, 0.f);
	TopDownCameraSettings.bInheritYaw = false;
	TopDownCameraSettings.bDoCollisionTest = false;
	TopDownCameraSettings.bEnableCameraLag = true;
	TopDownCameraSettings.CameraLagSpeed = 3.0f;
	TopDownCameraSettings.FieldOfView = 45.f;
	TopDownCameraSettings.bUseControllerRotationYaw = false;
	CameraTypeSettings.Add(ECameraType::TopDown, TopDownCameraSettings);

	LastMoveInput = FVector2D::ZeroVector;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

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
	ApplyNewCameraSettings(CameraTypeChangedPayload.NewCameraType);
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

void ACommonPlayerCharacter::ApplyNewCameraSettings(ECameraType NewCameraType)
{
	check(CameraTypeSettings.Contains(NewCameraType))
	const FCameraTypeSettings& NewCameraSettings = *CameraTypeSettings.Find(NewCameraType);
	SpringArmComponent->TargetArmLength = NewCameraSettings.TargetArmLength;
	SpringArmComponent->SocketOffset = NewCameraSettings.TargetArmOffset;
	SpringArmComponent->SetRelativeRotation(NewCameraSettings.TargetArmRotation);
	SpringArmComponent->bUsePawnControlRotation = NewCameraSettings.bSpringArmUsePawnControlRotation;
	SpringArmComponent->bEnableCameraLag = NewCameraSettings.bEnableCameraLag;
	SpringArmComponent->CameraLagSpeed = NewCameraSettings.CameraLagSpeed;
	SpringArmComponent->bEnableCameraRotationLag = NewCameraSettings.bEnableCameraRotationLag;
	SpringArmComponent->CameraRotationLagSpeed = NewCameraSettings.CameraRotationLagSpeed;
	SpringArmComponent->bDoCollisionTest = NewCameraSettings.bDoCollisionTest;
	SpringArmComponent->bInheritYaw = NewCameraSettings.bInheritYaw;
	SpringArmComponent->bInheritPitch = NewCameraSettings.bInheritPitch;
	SpringArmComponent->bInheritRoll = NewCameraSettings.bInheritRoll;

	CameraComponent->bUsePawnControlRotation = NewCameraSettings.bCameraUsePawnControlRotation;
	CameraComponent->SetFieldOfView(NewCameraSettings.FieldOfView);

	bUseControllerRotationYaw = NewCameraSettings.bUseControllerRotationYaw;
	bUseControllerRotationPitch = NewCameraSettings.bUseControllerRotationPitch;
	bUseControllerRotationRoll = NewCameraSettings.bUseControllerRotationRoll;
}
