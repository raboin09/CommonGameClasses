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
	USkeletalMeshComponent* GetMesh() const { return OwnerCharacter == nullptr ? nullptr : OwnerCharacter->GetMesh(); };
	UCapsuleComponent* GetCapsuleComponent() const { return OwnerCharacter == nullptr ? nullptr : OwnerCharacter->GetCapsuleComponent(); };
	UCharacterMovementComponent* GetCharacterMovement() const { return OwnerCharacter == nullptr ? nullptr : OwnerCharacter->GetCharacterMovement(); };
	FRotator GetActorRotation() const { return OwnerCharacter == nullptr ? FRotator() : OwnerCharacter->GetActorRotation(); };
	FVector GetActorLocation() const { return OwnerCharacter == nullptr ? FVector() : OwnerCharacter->GetActorLocation(); };
	
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
	void Internal_RagdollUpdate(float DeltaTime);
	FVector Internal_RagdollTraceGround() const;

	UPROPERTY()
	TWeakObjectPtr<ACommonCharacter> OwnerCharacter;
	UPROPERTY()
	TWeakObjectPtr<UCommonAnimInstance> OwnerAnimInstance;
	UPROPERTY()
	TWeakObjectPtr<UGameplayTagComponent> OwningTagComponent;
	UPROPERTY()
	FCharacterMontageEnded CharacterMontageEnded;	
	
	FRotator ControlRotation;

	// Used for async loading, will play anim after loaded
	TMap<TSoftObjectPtr<UAnimMontage>, FAnimMontagePlayData> CachedMontageData;

	// Ragdolling
	FVector RagdollMeshLocation;
	FTimerHandle TimerHandle_Ragdoll;
	FVector LastRagdollVelocity = FVector::ZeroVector;
public:
	FORCEINLINE TWeakObjectPtr<UCommonAnimInstance> GetAnimInstance() const { return OwnerAnimInstance; }
	FORCEINLINE FCharacterMontageEnded& OnCharacterMontageEnded() { return CharacterMontageEnded; }
	FORCEINLINE TWeakObjectPtr<UAnimMontage> GetCurrentPlayingMontage() const { return OwnerCharacter.IsValid() ? OwnerCharacter->GetCurrentMontage() : nullptr; }
};
