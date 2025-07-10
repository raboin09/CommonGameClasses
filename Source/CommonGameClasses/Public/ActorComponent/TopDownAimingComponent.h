// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownAimingComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Variable", "Sockets", "Tags", "ComponentReplication", "Component Replication", "Cooking", "Replication", "AssetUserData", "Asset User Data", "Navigation"))
class COMMONGAMECLASSES_API UTopDownAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTopDownAimingComponent();
	
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface

	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void StickAim(const FVector2D& InputVector);
	UFUNCTION(BlueprintCallable, Category="COMMON|Aiming")
	void MouseAim(const FVector2D& InputVector);
	void ToggleShouldRotateCharacterToAiming(bool bInShouldRotateCharacterToAiming);
	
protected:
	UPROPERTY(EditAnywhere, Category="Input")
	TEnumAsByte<ETraceTypeQuery> MouseAimTraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	
private:
	void DoAim(float AxisX, float AxisY);

	float CachedGamepadDeadZone = .1f;
	float CachedGamepadSensitivity = 1.f;
	bool bShouldRotateCharacterToAiming = false;
	float AimAngle = 0.0f;
	TWeakObjectPtr<APlayerController> PlayerController;
	bool bUsingMouse = false;
};
