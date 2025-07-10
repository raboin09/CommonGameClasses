// Fill out your copyright notice in the Description page of Project CUSTOM.

#pragma once

#include "CoreMinimal.h"
#include "Character/CommonCharacter.h"
#include "CommonPlayerCharacter.generated.h"

class USpringArmComponent;
enum class ECameraType : uint8;
class UCameraComponent;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("COMMON", "COMMON|Defaults"), PrioritizeCategories = ("COMMON"))
class COMMONGAMECLASSES_API ACommonPlayerCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	ACommonPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	// AxisX is 'Forward', AxisY is 'Right'
	UFUNCTION(BlueprintCallable, Category="COMMON|PlayerCharacter")
	void MoveInput(float AxisX, float AxisY);
	UFUNCTION(BlueprintCallable, Category="COMMON|PlayerCharacter")
	void MouseInput(float Yaw, float Pitch);
	
protected:	
	//~ Begin ACharacter interface implementation
	virtual void PossessedBy(AController* NewController) override;
	//~ End ACharacter interface implementation

	// AxisX is 'Forward', AxisY is 'Right'
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerCharacter")
	void BPI_MoveInput(float AxisX, float AxisY);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerCharacter")
	void BPI_MouseInput(float Yaw, float Pitch);

	UFUNCTION()
	void HandleCameraTypeChanged(const FCameraTypeChangedPayload& CameraTypeChangedPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerCharacter")
	void BPI_CameraChanged(const FCameraTypeChangedPayload& CameraTypeChangedPayload);

	virtual void SetupThirdPersonCamera();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerCharacter")
	void BPI_SetupThirdPersonCamera();
	
	virtual void SetupFirstPersonCamera();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerCharacter")
	void BPI_SetupFirstPersonCamera();
	
	virtual void SetupTopDownCamera();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerCharacter")
	void BPI_SetupTopDownCamera();
	
private:
	void Internal_GetMoveDirections(FVector& OutForwardMoveDirection, FVector& OutRightMoveDirection) const;

	float CachedGamepadDeadZone = 0.f;
	float CachedGamepadSensitivity = 1.f;
};
