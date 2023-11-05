// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void SetAnimationOverlay(const FGameplayTag& NewOverlay);
	void StartRagdolling();
	void StopRagdolling();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ACommonCharacter* OwnerCharacter;
	UPROPERTY()
	UGameplayTagComponent* OwningTagComponent;
	
private:
	FRotator ControlRotation;
	
};
