// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAnimInstance.generated.h"

struct FGameplayTag;
class ACommonCharacter;
class UCommonCharacterMovementComponent;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UCommonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//~ Begin UAnimInstance Interface
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ End UAnimInstance Interface
	
protected:
	UFUNCTION(BlueprintPure, Category="COMMON|Animation")
	bool AnimOwnerHasGameplayTag(const FGameplayTag Tag) const;
	
	UPROPERTY(BlueprintReadOnly, Category = "COMMON|Character State Data")
	float GroundDistance = -1.0f;

private:
	bool Internal_AssignRequiredVars();
	
	TWeakObjectPtr<UCommonCharacterMovementComponent> CommonCharacterMovementComponent;
	TWeakObjectPtr<ACommonCharacter> CommonCharacter;
};
