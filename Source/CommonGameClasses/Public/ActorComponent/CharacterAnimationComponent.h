// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CommonCharacter.h"
#include "Components/ActorComponent.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Types/CommonEventDeclarations.h"
#include "CharacterAnimationComponent.generated.h"

class UCommonAnimInstance;

namespace CommonGameAnimation
{
	static FName NAME_Pelvis = TEXT("Pelvis"); 
}

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Variable", "Sockets", "Tags", "ComponentReplication", "Component Replication", "Cooking", "Replication", "AssetUserData", "Asset User Data", "Navigation"))
class COMMONGAMECLASSES_API UCharacterAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//~ Begin UActorComponent interface implementation
	UCharacterAnimationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	//~ End UActorComponent interface implementation

	/**
	 * Attempts to play the specified animation montage on the character if no montage is currently playing.
	 *
	 * @param AnimMontageData The data containing the montage to play, section name, play rate, and other specifications.
	 * @return The length of the animation montage being played in seconds if successful, or -1.0f if the montage could not be played.
	 */
	float TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData);
	
	/**
	 * Stops the specified animation montage playing on the character, or stops the currently playing montage if none is specified.
	 *
	 * @param Montage The animation montage to stop. If nullptr, the currently playing montage will be stopped.
	 */
	void StopAnimMontage(UAnimMontage* Montage = nullptr);
	
	/**
	 * Forces the specified animation montage to play on the character, overriding any currently playing montage.
	 *
	 * @param AnimMontageData The data including the montage to play, section name, play rate, and other specifications.
	 * @return The length of the animation montage being played in seconds if successful, or -1.0f if the montage could not be played.
	 */
	float ForcePlayAnimMontage(const FAnimMontagePlayData& AnimMontageData);

	void SetAnimationOverlay(const FGameplayTag& NewOverlay);
	/**
	 * Initiates ragdoll physics on the character, disabling movement and animation, and enabling physics simulation.
	 *
	 * This method applies necessary changes to the character's mesh and physics components to transition into a ragdoll state.
	 * It broadcasts a ragdoll event upon successful initiation.
	 *
	 * - Tags the character as being in a ragdoll state.
	 * - Stops any currently playing animation montage.
	 * - Modifies the mesh collision type and enables physics simulation for all bodies below the pelvis.
	 * - Disables the character's movement and collision on the capsule component.
	 * - Enables ticking for the component to handle ragdoll-related updates.
	 *
	 * @see StopAnimMontage
	 * @see OnCharacterRagdollingEvent
	 */
	
	UFUNCTION(BlueprintCallable)
	void StartRagdolling();
	/**
	 * Stops the character's ragdoll state and restores normal movement, collision, and animation behavior.
	 *
	 * This method removes the ragdoll-related tag from the character, re-enables collision for the capsule component,
	 * and switches the character's movement mode back to walking. It also restores the character's mesh to its original
	 * position and rotation and disables physics simulation for all bodies below the pelvis. Finally, it disables ticking
	 * for the component and broadcasts an event signaling the end of the ragdoll state.
	 *
	 * - Removes the "Ragdoll" gameplay tag from the character.
	 * - Restores collision and movement for character components.
	 * - Repositions and reorients the character's mesh to its cached state.
	 * - Disables physics simulation below the pelvis.
	 * - Broadcasts an updated ragdoll state event.
	 *
	 * @see OnCharacterRagdollingEvent
	 */
	
	UFUNCTION(BlueprintCallable)
	void StopRagdolling();

private:	
	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* EndedMontage, bool bInterrupted);
	UFUNCTION()
	void HandleCurrentWoundChangedEvent(const FCurrentWoundEventPayload& CurrentWoundEventPayload);
	UFUNCTION()
	void HandleActorDeathEvent(const FActorDeathEventPayload& DeathEventPayload);
	UFUNCTION()
	void HandleCameraTypeChanged(const FCameraTypeChangedPayload& CameraTypeChangedPayload);
	
	float Internal_PlayMontage(const FAnimMontagePlayData& AnimMontagePlayData);
	void Internal_ApplyCharacterKnockback(const FVector& Impulse, const float ImpulseScale, const FName BoneName, bool bVelocityChange);
	void Internal_TryStartCharacterKnockback(const FDamageHitReactEvent& HitReactEvent, bool bIsDeathKnockback = false);
	void Internal_TryPlayHitReact(const FDamageHitReactEvent& HitReactEvent);
	FGameplayTag Internal_GetHitDirectionTag(const FVector& OriginatingLocation) const;
	void Internal_TryCharacterKnockbackRecovery();
	void Internal_RagdollUpdate(float DeltaTime);
	FVector Internal_RagdollTraceGround() const;
	void Internal_ChangeRotation(ERotationMethod RotationMethod);

	UPROPERTY()
	TWeakObjectPtr<ACommonCharacter> OwnerCharacter;
	UPROPERTY()
	TWeakObjectPtr<UCommonAnimInstance> OwnerAnimInstance;
	UPROPERTY()
	TWeakObjectPtr<UGameplayTagComponent> OwningTagComponent;
	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Character Montage Ended")
	FCharacterMontageEnded CharacterMontageEnded;
	UPROPERTY(BlueprintAssignable, DisplayName="COMMON On Character Ragdolling Event")
	FCharacterRagdollingEvent CharacterRagdolling;
	
	FRotator ControlRotation;

	// Ragdolling
	FVector RagdollMeshLocation;
	FTimerHandle TimerHandle_Ragdoll;
	FVector LastRagdollVelocity = FVector::ZeroVector;
	FVector CachedMeshOffset;
	FRotator CachedMeshRotation;

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return OwnerCharacter == nullptr ? nullptr : OwnerCharacter->GetMesh(); };
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return OwnerCharacter == nullptr ? nullptr : OwnerCharacter->GetCapsuleComponent(); };
	FORCEINLINE UCharacterMovementComponent* GetCharacterMovement() const { return OwnerCharacter == nullptr ? nullptr : OwnerCharacter->GetCharacterMovement(); };
	FORCEINLINE FRotator GetActorRotation() const { return OwnerCharacter == nullptr ? FRotator() : OwnerCharacter->GetActorRotation(); };
	FORCEINLINE FVector GetActorLocation() const { return OwnerCharacter == nullptr ? FVector() : OwnerCharacter->GetActorLocation(); };
public:
	FORCEINLINE TWeakObjectPtr<UCommonAnimInstance> GetAnimInstance() const { return OwnerAnimInstance; }
	FORCEINLINE TWeakObjectPtr<UAnimMontage> GetCurrentPlayingMontage() const { return OwnerCharacter.IsValid() ? OwnerCharacter->GetCurrentMontage() : nullptr; }
	FORCEINLINE FCharacterMontageEnded& OnCharacterMontageEnded() { return CharacterMontageEnded; }
	FORCEINLINE FCharacterRagdollingEvent& OnCharacterRagdollingEvent() { return CharacterRagdolling; }
};
