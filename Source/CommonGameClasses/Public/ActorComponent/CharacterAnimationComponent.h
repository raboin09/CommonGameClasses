// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CommonCharacter.h"
#include "Components/ActorComponent.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Types/CommonEventDeclarations.h"
#include "CharacterAnimationComponent.generated.h"

class UCommonAnimInstance;

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
	UFUNCTION(BlueprintCallable)
	void StartRagdolling();
	UFUNCTION(BlueprintCallable)
	void StopRagdolling();
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* EndedMontage, bool bInterrupted);
	UFUNCTION()
	void HandleCurrentWoundChangedEvent(const FCurrentWoundEventPayload& CurrentWoundEventPayload);
	UFUNCTION()
	void HandleActorDeathEvent(const FActorDeathEventPayload& DeathEventPayload);
	UFUNCTION()
	float HandleMontageLoadedEvent(TSoftObjectPtr<UAnimMontage> LoadedAnimMontage);
	
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
	TWeakObjectPtr<UAnimInstance> OwnerAnimInstance;
	UPROPERTY()
	UGameplayTagComponent* OwningTagComponent;
	UPROPERTY()
	FCharacterMontageEnded CharacterMontageEnded;	
	
	FRotator ControlRotation;
	TMap<TSoftObjectPtr<UAnimMontage>, FAnimMontagePlayData> CachedMontageData;

	// Ragdolling
	FTimerHandle TimerHandle_Ragdoll;
	FVector LastRagdollVelocity = FVector::ZeroVector;

public:
	FORCEINLINE TWeakObjectPtr<UAnimInstance> GetAnimInstance() const { return OwnerAnimInstance; }
	FORCEINLINE FCharacterMontageEnded& OnCharacterMontageEnded() { return CharacterMontageEnded; }
	FORCEINLINE TWeakObjectPtr<UAnimMontage> GetCurrentPlayingMontage() const { return OwnerCharacter ? OwnerCharacter->GetCurrentMontage() : nullptr; }
};
