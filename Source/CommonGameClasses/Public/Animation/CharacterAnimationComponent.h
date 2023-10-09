// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAnimInstance.h"
#include "Character/CommonCharacter.h"
#include "Components/ActorComponent.h"
#include "Types/CommonEventDeclarations.h"
#include "CharacterAnimationComponent.generated.h"

struct FAnimMontagePlayData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UCharacterAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAnimationComponent();
	
	float TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	void HandleTagAdded(const FGameplayTagAddedEventPayload& GameplayTagAddedEventPayload);

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	ERotationMethod RotationMethod = ERotationMethod::NONE;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	float RotationSpeed = 360.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	float TurnStartAngle = 90.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	float TurnStopTolerance = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	EAimOffsets AimOffsetType = EAimOffsets::NONE;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	EAimOffsetClamp AimOffsetBehavior = EAimOffsetClamp::Nearest;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	float AimClamp = 135.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup")
	bool CameraBased;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup", meta=(EditCondition = "AimOffsetType == EAimOffsets::Aim"))
	FName AimSocketName = "hand_r";
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "CUSTOM|Setup", meta=(EditCondition = "AimOffsetType == EAimOffsets::Look"))
	FName LookAtSocketName = "Spine_03";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CUSTOM|Runtime")
	FRotator AimOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CUSTOM|Runtime")
	FVector LookAtLocation;

	UPROPERTY()
	ACommonCharacter* OwnerCharacter;
	UPROPERTY()
	UCommonAnimInstance* OwnerAnimInstance;
	UPROPERTY()
	UGameplayTagComponent* OwningTagComponent;
	
private:
	FRotator ControlRotation;
	
};
