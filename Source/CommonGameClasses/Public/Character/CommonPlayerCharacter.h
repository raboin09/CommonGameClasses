﻿// Fill out your copyright notice in the Description page of Project CUSTOM.

#pragma once

#include "CoreMinimal.h"
#include "Character/CommonCharacter.h"
#include "CommonPlayerCharacter.generated.h"

class USpringArmComponent;
enum class ECameraType : uint8;
class UCameraComponent;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM", "CUSTOM|Defaults"), PrioritizeCategories = "CUSTOM")
class COMMONGAMECLASSES_API ACommonPlayerCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	ACommonPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	
	//~ Begin ACharacter interface implementation
	virtual void PossessedBy(AController* NewController) override;
	//~ End ACharacter interface implementation
	
	UFUNCTION(BlueprintCallable, Category="COMMON|PlayerCharacter")
	void MoveInput(float AxisX, float AxisY);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerCharacter")
	void BPI_MoveInput(float AxisX, float AxisY);
	
	UFUNCTION(BlueprintCallable, Category="COMMON|PlayerCharacter")
	void MouseInput(float Yaw, float Pitch);
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
	void ApplyNewCameraSettings(ECameraType NewCameraType);

protected:
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Camera")
	TMap<ECameraType, FCameraTypeSettings> CameraTypeSettings;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COMMON|PlayerCharacter")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COMMON|PlayerCharacter")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
private:
	FVector2D LastMoveInput;
};
