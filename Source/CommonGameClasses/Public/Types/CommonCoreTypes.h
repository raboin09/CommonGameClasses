// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCoreTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCommonGameClasses, Log, All);

#define CUR_CLASS_FUNC (FString(__FUNCTION__))
#define CUR_LINE  (FString::FromInt(__LINE__))
#define CUR_CLASS_FUNC_LINE (CUR_CLASS_FUNC + "(" + CUR_LINE + ")")

#define COMMON_LOG(LogVerbosity, Param1) UE_LOG(LogCommonGameClasses, LogVerbosity, TEXT("%s: %s"), *CUR_CLASS_FUNC_LINE, *FString(Param1))
#define COMMON_PRINT_SCREEN_RED(Message, Duration) GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, FString(Message));
#define COMMON_PRINT_SCREEN_GREEN(Message, Duration) GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Green, FString(Message));

#define COMMON_TRACE_INTERACTION ECC_GameTraceChannel1
#define COMMON_TRACE_ABILITY ECC_GameTraceChannel2
#define COMMON_OBJECT_TYPE_PROJECTILE ECC_GameTraceChannel3

#define COMMON_SURFACE_Default		SurfaceType_Default
#define COMMON_SURFACE_Concrete	SurfaceType1
#define COMMON_SURFACE_Dirt		SurfaceType2
#define COMMON_SURFACE_Water		SurfaceType3
#define COMMON_SURFACE_Metal		SurfaceType4
#define COMMON_SURFACE_Wood		SurfaceType5
#define COMMON_SURFACE_Grass		SurfaceType6
#define COMMON_SURFACE_Glass		SurfaceType7
#define COMMON_SURFACE_Flesh		SurfaceType8
#define COMMON_SURFACE_Plastic		SurfaceType12
#define COMMON_SURFACE_Sand		SurfaceType13
#define COMMON_SURFACE_Ice		SurfaceType14

UENUM(BlueprintType)
enum class ECameraType : uint8
{
	None,
	FirstPerson,
	ThirdPerson,
	TopDown
};

USTRUCT(BlueprintType)
struct FCameraTypeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")	
	float TargetArmLength = 300.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")
	FRotator TargetArmRotation = FRotator(0.f, 0.f, 0.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")
	FVector TargetArmOffset = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")
	bool bSpringArmUsePawnControlRotation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")
	bool bEnableCameraLag = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm", meta=(editcondition="bEnableCameraLag", ClampMin="0.0", ClampMax="1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float CameraLagSpeed = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")
	bool bEnableCameraRotationLag = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm", meta=(editcondition = "bEnableCameraRotationLag", ClampMin="0.0", ClampMax="1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float CameraRotationLagSpeed = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")
	bool bDoCollisionTest = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	bool bInheritYaw = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	bool bInheritPitch = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	bool bInheritRoll = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	bool bCameraUsePawnControlRotation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta=(ClampMin="45", ClampMax="90"))
	float FieldOfView = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	bool bUseControllerRotationYaw = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	bool bUseControllerRotationPitch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	bool bUseControllerRotationRoll = false;
};
