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
	/**
	 * Retrieves the forward direction vector of the character based on the current camera type.
	 * The forward direction varies depending on whether the character is in first-person,
	 * third-person, or top-down camera mode.
	 *
	 * @return A FVector representing the forward direction of the character. It returns a zero vector
	 * if the camera type is not recognized or no valid camera component is found.
	 */
	void GetCardinalDirections(FVector& OutForwardDirection, FVector& OutRightDirection) const;
	
protected:	
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

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

	virtual UCameraComponent* GetCurrentCameraComponent() const; 
	
private:
	void Internal_GetMoveDirections(FVector& OutForwardMoveDirection, FVector& OutRightMoveDirection) const;

	float CachedGamepadDeadZone = 0.f;
	float CachedGamepadSensitivity = 1.f;
};
