// Fill out your copyright notice in the Description page of Project Settings.

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
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface

	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void MoveInput(const FVector2D& InputVector);
	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void GamepadAimInput(const FVector2D& InputVector);
	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void MouseAimInput(const FVector2D& InputVector);
	
	void ToggleUseTwinStickAiming(bool bShouldUseTwinStickAiming);

	/**
	 * Snaps the actor's rotation to align with the current aiming direction without interpolation.
	 *
	 * Behavior:
	 * - If a gamepad is being used, calculates the aim angle based on the last gamepad input
	 *   and rotates the character to align with the gamepad aim direction.
	 * - If a mouse is being used, rotates the character to align with the mouse aim direction.
	 *
	 * This method is typically used to immediately adjust the actor's orientation
	 * to match the current aiming direction, often in response to specific player actions
	 * or ability activations.
	 */
	void SnapRotateActorToAim();
	void ToggleMovementOrientRotation(bool bOrientRotationToMovement);
	
protected:
	UPROPERTY(EditAnywhere, Category="COMMON")
	TEnumAsByte<ETraceTypeQuery> MouseAimTraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	UPROPERTY(EditAnywhere, Category="COMMON")
	float RotationInterpSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category="COMMON")
	float MinRotationRate = 0.0f;
	UPROPERTY(EditAnywhere, Category="COMMON")
	float MaxRotationRate = 720.0f;
	UPROPERTY(BlueprintReadOnly, Category="COMMON|Input")
	FVector2D LastMoveInput;
	UPROPERTY(BlueprintReadOnly, Category="COMMON|Input")
	FVector2D LastAimInput;
	
private:
	void CalculateAimAngle(float AxisX, float AxisY);

	void Internal_MoveInput(const FVector2D& InputVector);
	bool Internal_AdjustGamepadInputs(float& OutAxisX, float& OutAxisY) const;
	void Internal_TryAssignDeadzoneInput(float& OutAxisX, float& OutAxisY) const;
	void Internal_AdjustInputSensitivity(float& OutAxisX, float& OutAxisY) const;
	FRotator GetMouseRotation() const;
	static bool AreInputsValid(const float AxisX, const float AxisY);
	void Internal_RotateCharacterToMouse(bool bInterpRotation);
	void Internal_RotateCharacterToGamepad(bool bInterpRotation);
	void TryToggleUsingGamepad(bool bInUsingGamepad);

	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Gamepad Input Changed")
	FGamepadInputChanged GamepadInputChanged;

	float CachedGamepadDeadZone = .1f;
	float CachedGamepadSensitivity = 1.f;
	bool bShouldRotateCharacterToAiming = false;
	float AimAngle = 0.0f;
	
	bool bUsingGamepad = true;

	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<ACommonPlayerCharacter> PlayerCharacter;

public:
	FORCEINLINE FGamepadInputChanged& OnMouseGamepadInputChanged() { return GamepadInputChanged; }
};
