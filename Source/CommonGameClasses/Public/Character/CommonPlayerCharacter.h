// Fill out your copyright notice in the Description page of Project CUSTOM.

#pragma once

#include "CoreMinimal.h"
#include "AlsCameraComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Character/CommonCharacter.h"
#include "CommonPlayerCharacter.generated.h"

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM", "CUSTOM|Defaults"), PrioritizeCategories = "CUSTOM")
class COMMONGAMECLASSES_API ACommonPlayerCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	ACommonPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CUSTOM")
	TObjectPtr<UAlsCameraComponent> Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CUSTOM|Input", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CUSTOM|Input", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LookMouseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CUSTOM|Input", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CUSTOM|Input", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CUSTOM|Input", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Input", Meta = (ClampMin = 0, ForceUnits = "x"))
	float LookUpMouseSensitivity{1.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Input", Meta = (ClampMin = 0, ForceUnits = "x"))
	float LookRightMouseSensitivity{1.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Input", Meta = (ClampMin = 0, ForceUnits = "deg/s"))
	float LookUpRate{90.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Input", Meta = (ClampMin = 0, ForceUnits = "deg/s"))
	float LookRightRate{240.0f};
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo) override;
	virtual void NotifyControllerChanged() override;
	
private:
	void Input_OnLookMouse(const FInputActionValue& ActionValue);
	void Input_OnLook(const FInputActionValue& ActionValue);
	void Input_OnMove(const FInputActionValue& ActionValue);
	void Input_OnAim(const FInputActionValue& ActionValue);
};
