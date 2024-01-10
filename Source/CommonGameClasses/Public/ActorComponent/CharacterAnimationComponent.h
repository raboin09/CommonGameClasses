﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	void StopAnimMontage(UAnimMontage* Montage = nullptr);
	float ForcePlayAnimMontage(const FAnimMontagePlayData& AnimMontageData);
	
	void SetAnimationOverlay(const FGameplayTag& NewOverlay);
	void StartRagdolling();
	void StopRagdolling();
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

private:
	float Internal_PlayMontage(const FAnimMontagePlayData& AnimMontagePlayData);
	void Internal_ApplyCharacterKnockback(const FVector& Impulse, const float ImpulseScale, const FName BoneName, bool bVelocityChange);
	void Internal_TryStartCharacterKnockback(const FDamageHitReactEvent& HitReactEvent, bool bShouldRecoverFromKnockback = true);
	void Internal_TryPlayHitReact(const FDamageHitReactEvent& HitReactEvent);
	FGameplayTag Internal_GetHitDirectionTag(const FVector& OriginatingLocation) const;
	void Internal_TryCharacterKnockbackRecovery();
	void Internal_RagdollUpdate();
	
	UPROPERTY()
	ACommonCharacter* OwnerCharacter;
	UPROPERTY()
	UGameplayTagComponent* OwningTagComponent;
	FRotator ControlRotation;

	// Ragdolling
	FTimerHandle TimerHandle_Ragdoll;
	FVector LastRagdollVelocity = FVector::ZeroVector;
};
