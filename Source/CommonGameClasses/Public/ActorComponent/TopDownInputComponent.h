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
	 * Attempts to rotate the owning actor towards the current mouse position in the game world.
	 *
	 * This method is intended to handle actor rotation when using mouse input for aiming.
	 * It calculates the rotation needed to face the current mouse cursor position projected onto
	 * the game world, and updates the actor's rotation accordingly.
	 *
	 * Rotation will be performed immediately without interpolation.
	 * The method uses the internal helper method `Internal_RotateCharacterToMouse` with interpolation disabled (`bInterpRotation` set to false).
	 *
	 * Preconditions:
	 * - The mouse input feature must be active (`bUsingMouse` set to true).
	 * - `PlayerController` must be valid.
	 *
	 * Postconditions:
	 * - If the mouse cursor hits a valid traceable location and a certain distance threshold is met, the actor's rotation
	 *   will be updated to face that location.
	 * - If conditions are not met, the actor will retain its current rotation.
	 */
	void TryRotateActorToMouse();
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
	
private:
	void DoAim(float AxisX, float AxisY);

	void Internal_MoveInput(const FVector2D& InputVector);
	bool Internal_AdjustGamepadInputs(float& OutAxisX, float& OutAxisY) const;
	void Internal_TryAssignDeadzoneInput(float& OutAxisX, float& OutAxisY) const;
	void Internal_AdjustInputSensitivity(float& OutAxisX, float& OutAxisY) const;
	FRotator GetMouseRotation() const;
	static bool AreInputsValid(const float AxisX, const float AxisY);
	void Internal_RotateCharacterToMouse(bool bInterpRotation);
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|Aiming")
	FORCEINLINE bool IsUsingMouse(){ return bUsingGamepad; }
	FORCEINLINE FGamepadInputChanged& OnMouseGamepadInputChanged() { return GamepadInputChanged; }
};
