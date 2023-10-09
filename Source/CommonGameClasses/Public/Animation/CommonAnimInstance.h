// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "CommonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void DisableRootMotionModeForDuration(float Duration);
	
protected:
	UCommonAnimInstance();
	virtual void NativeInitializeAnimation() override;
	// TODO Move to thread safe update 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void GetCharacterMovementData();
	void GetMovementStates();
	
	void InitCharacterComponents();
	
public:
	////////////////////////////////////////////////////
	/// Character
	////////////////////////////////////////////////////
	
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Character")
	ACharacter* OwningCharacter;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Character")
	class UCharacterMovementComponent* OwningMovementComponent;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Character")
	class UCharacterAnimationComponent* CharacterAnimationComponent;

	////////////////////////////////////////////////////
	/// Core
	////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	float DeltaTick;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	ERotationMethod RotationMethod;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	FGameplayTag BasePose;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	FGameplayTag OverlayPose;
	
	////////////////////////////////////////////////////
	/// Locomotion
	////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	float Velocity;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	float ForwardVelocity;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	float UpVelocity;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	float LeftVelocity;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	float Direction;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	FVector InputAcceleration;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	FRotator PreviousRotation;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|Locomotion")
	FRotator PreviousFrameAim;
	
	////////////////////////////////////////////////////
	/// States
	////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsIdle;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsStanding;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsCrouching;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsGrounded;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsWalkingState;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsFlying;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	bool bIsFalling;
	UPROPERTY(BlueprintReadOnly, Category="CUSTOM|States")
	TEnumAsByte<EMovementMode> MovementMode;

private:
	UFUNCTION()
	void EnableRootMotionMode();
	
	ERootMotionMode::Type RootMotionCache;
	FTimerHandle TimerHandle_RootMotionDisable;
};
