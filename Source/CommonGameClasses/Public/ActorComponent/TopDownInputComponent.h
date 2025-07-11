﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "TopDownInputComponent.generated.h"

class ACommonPlayerCharacter;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Variable", "Sockets", "Tags", "ComponentReplication", "Component Replication", "Cooking", "Replication", "AssetUserData", "Asset User Data", "Navigation"))
class COMMONGAMECLASSES_API UTopDownInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTopDownInputComponent();
	
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface

	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void MoveInput(const FVector2D& InputVector);
	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void GamepadAimInput(const FVector2D& InputVector);
	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void MouseAimInput(const FVector2D& InputVector);
	
	void ToggleGamepadRootedAimingMode(bool bInGamepadRootedAimingMode);
	void ToggleMovementOrientRotation(bool bOrientRotationToMovement);
	
protected:
	UPROPERTY(EditAnywhere, Category="COMMON")
	float RotationInterpSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category="COMMON")
	float MinRotationRate = 0.0f;
	UPROPERTY(EditAnywhere, Category="COMMON")
	float MaxRotationRate = 720.0f;
	
private:
	void Internal_MoveInput(const FVector2D& InputVector);
	bool Internal_AdjustGamepadInputs(float& OutAxisX, float& OutAxisY) const;
	void Internal_TryAssignDeadzoneInput(float& OutAxisX, float& OutAxisY) const;
	void Internal_AdjustInputSensitivity(float& OutAxisX, float& OutAxisY) const;
	FRotator GetMouseRotation() const;
	static bool AreInputsValid(const float AxisX, const float AxisY);
	
	void Internal_RotateCharacterToMouse(bool bInterpRotation) const;
	void Internal_RotateCharacterToGamepad(bool bInterpRotation) const;
	void Internal_RotateCharacter(const FRotator& TargetRotation, bool bShouldInterpRotation) const;

	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Gamepad Input Changed")
	FGamepadInputChanged GamepadInputChanged;

	float CachedGamepadDeadZone = .1f;
	float CachedGamepadSensitivity = 1.f;
	bool bGamepadRootedAimingMode = false;
	
	bool bWasUsingGamepad = true;

	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<ACommonPlayerCharacter> PlayerCharacter;

public:
	FORCEINLINE FGamepadInputChanged& OnMouseGamepadInputChanged() { return GamepadInputChanged; }
};
