// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/TopDownAimingComponent.h"

#include "Core/CommonCoreDeveloperSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UTopDownAimingComponent::UTopDownAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UTopDownAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	if(const UCommonCoreDeveloperSettings* GameSettings = GetDefault<UCommonCoreDeveloperSettings>())
	{
		CachedGamepadDeadZone = GameSettings->GamePadDeadZone;
		CachedGamepadSensitivity = GameSettings->GamePadSensitivity;	
	}
}

void UTopDownAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!PlayerController.IsValid())
	{
		PlayerController = GetWorld()->GetFirstPlayerController();
	}

	if(!bShouldRotateCharacterToAiming)
	{
		return;
	}
	
	const FRotator OldRotation = GetOwner()->GetActorRotation();
	if (bUsingMouse)
	{
		if (PlayerController.IsValid())
		{
			FHitResult OutHit; 
			PlayerController->GetHitResultUnderCursorByChannel(MouseAimTraceChannel, true, OutHit);
			if(FVector::Distance(OutHit.Location, GetOwner()->GetActorLocation()) > 50.f)
			{
				const FRotator AimRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), OutHit.Location);
				AimAngle = AimRot.Yaw;
				FRotator TargetRotation = FRotator(OldRotation.Pitch, AimAngle, OldRotation.Roll);
				const FRotator NewRotation = FMath::RInterpTo(OldRotation, TargetRotation, DeltaTime, 5.f);
				GetOwner()->SetActorRotation(NewRotation);	
			}
		}

	} else {
		const FRotator TargetRot = FRotator(OldRotation.Pitch, -AimAngle, OldRotation.Roll);
		const FRotator NewRotation = FMath::RInterpTo(OldRotation, TargetRot, DeltaTime, 5.f);
		GetOwner()->SetActorRotation(NewRotation);
	}
}

void UTopDownAimingComponent::DoAim(float AxisX, float AxisY)
{
	if(!bShouldRotateCharacterToAiming)
	{
		return;
	}
	
	AimAngle = FMath::RadiansToDegrees(FMath::Atan2(AxisY, -AxisX));
	bUsingMouse = false;
	if (PlayerController.IsValid())
	{
		PlayerController->SetShowMouseCursor(false);
	}
}

void UTopDownAimingComponent::StickAim(const FVector2D& InputVector)
{
	if(!bShouldRotateCharacterToAiming)
	{
		return;
	}
	float OutAxisX = InputVector.X * CachedGamepadSensitivity;
	float OutAxisY = InputVector.Y * CachedGamepadSensitivity;
	
	if(FMath::Abs(OutAxisX) < CachedGamepadDeadZone)
	{
		OutAxisX = 0;
	}
	
	if(FMath::Abs(OutAxisY) < CachedGamepadDeadZone)
	{
		OutAxisY = 0;
	}

	if(OutAxisX == 0 && OutAxisY == 0)
	{
		return;
	}
	DoAim(OutAxisX, OutAxisY);
}

void UTopDownAimingComponent::MouseAim(const FVector2D& InputVector)
{
	if(!bShouldRotateCharacterToAiming)
	{
		return;
	}
	bUsingMouse = true;
	// if (PlayerController.IsValid())
	// {
	// 	PlayerController->SetShowMouseCursor(true);
	// }
}

void UTopDownAimingComponent::ToggleShouldRotateCharacterToAiming(bool bInShouldRotateCharacterToAiming)
{
	bShouldRotateCharacterToAiming = bInShouldRotateCharacterToAiming;
	if(UCharacterMovementComponent* CharacterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>())
	{
		CharacterMovementComponent->bOrientRotationToMovement = !bShouldRotateCharacterToAiming;
	}
	if(APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		Pawn->bUseControllerRotationYaw = !bShouldRotateCharacterToAiming;
	}
	SetComponentTickEnabled(bShouldRotateCharacterToAiming);
}
