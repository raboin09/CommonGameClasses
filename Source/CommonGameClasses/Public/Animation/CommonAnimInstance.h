// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAnimInstanceProxy.h"
#include "CommonAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UCommonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Tag Bindings")
	bool bGameplayTag_IsReady = false;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Tag Bindings")
	bool bGameplayTag_IsDashing = false;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Tag Bindings")
	bool bGameplayTag_IsFiring = false;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Tag Bindings")
	bool bGameplayTag_IsAiming = false;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Tag Bindings")
	bool bGameplayTag_IsMelee = false;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Tag Bindings")
	bool bGameplayTag_IsSprinting = false;
	FORCEINLINE virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return new FCommonAnimationInstanceProxy(this); }
};
