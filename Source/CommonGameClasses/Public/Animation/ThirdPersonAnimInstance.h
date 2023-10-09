// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAnimInstance.h"
#include "ThirdPersonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UThirdPersonAnimInstance : public UCommonAnimInstance
{
	GENERATED_BODY()

protected:
	UThirdPersonAnimInstance();
	// TODO Move to thread safe update
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void GetAimingData();
	void GetLeaningData();
	
public:
	////////////////////////////////////////////////////
	/// Aim
	////////////////////////////////////////////////////
	
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|AimOffset")
	FRotator RawAimOffset;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|AimOffset")
	EAimOffsets AimOffsetType;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|AimOffset")
	EAimOffsetClamp AimOffsetBehavior;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|AimOffset")
	FVector LookAtLocation;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|AimOffset")
	FVector2D FinalAimOffset;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|AimOffset")
	float AimClamp;

	////////////////////////////////////////////////////
	/// Leans
	////////////////////////////////////////////////////
	
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|Leans")
	float Lean;
	UPROPERTY(BlueprintReadOnly, Category = "CUSTOM|Leans")
	FVector2D AimDelta;

	////////////////////////////////////////////////////
	/// Defaults
	////////////////////////////////////////////////////
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Defaults")
	float TargetFrameRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Defaults")
	float LeanSmooth;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Defaults")
	float AimSmooth;

private:
	FORCEINLINE float NormalizeLean(const float InValue) const { return (InValue * (1.0 / DeltaTick)) / TargetFrameRate; }
};
