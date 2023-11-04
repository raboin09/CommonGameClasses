﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "Ability/CommonAbility.h"
#include "API/Taggable.h"
#include "Types/CommonTagTypes.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "CommonCharacter.generated.h"

struct FGameplayTag;
class IMountable;
class UHealthComponent;
class UGameplayTagComponent;
class UCommonCharacterMovementComponent;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API ACommonCharacter : public AAlsCharacter, public ITaggable
{
	GENERATED_BODY()

public:	
	ACommonCharacter(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload) override;
	
	////////////////////////////////
	/// Common Events
	////////////////////////////////
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "COMMON|Character")
	void K2_OnDeath();
	virtual void K2_OnDeath_Implementation();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload);

	////////////////////////////////
	/// Common Variables
	////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Defaults")
	TMap<FGameplayTag, TSubclassOf<ACommonAbility>> DefaultAbilities;

	UPROPERTY()
	UAbilityComponent* AbilityComponent;
	UPROPERTY()
	class UEffectContainerComponent* EffectContainerComponent;
	UPROPERTY()
	class UCharacterAnimationComponent* CharacterAnimationComponent;
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;

	////////////////////////////////
	/// Mounts
	////////////////////////////////
public:
	FORCEINLINE void AssignNewMountable(UObject* InMountableObject, const FHitResult& InHitResult);
	FORCEINLINE TScriptInterface<IMountable> GetCurrentMount() const { return CurrentMount; }
	FORCEINLINE bool IsMounted() const { return CurrentMount != nullptr; }
	FORCEINLINE bool CanGetInCover() { return !UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::CannotMount); }
protected:
	UPROPERTY()
	TScriptInterface<IMountable> CurrentMount;

	////////////////////////////////
	/// Knockbacks and Hit Reacts
	////////////////////////////////
protected:
	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* K2_GetHitReactAnimation(const FGameplayTag& HitReactDirection) const;
	
private:
	void Internal_ApplyCharacterKnockback(const FVector& Impulse, const float ImpulseScale, const FName BoneName, bool bVelocityChange);
	void Internal_TryStartCharacterKnockback(const FDamageHitReactEvent& HitReactEvent, bool bShouldRecoverFromKnockback = true);
	void Internal_TryPlayHitReact(const FDamageHitReactEvent& HitReactEvent);
	FGameplayTag Internal_GetHitDirectionTag(const FVector& OriginatingLocation) const;

	FTimerHandle TimerHandle_HideWeapons;
	FTimerHandle TimerHandle_Ragdoll;	
	UPROPERTY(Transient)
	EHitReactType LastKnownHitReact;

	EVisibilityBasedAnimTickOption DefVisBasedTickOp;
	bool bRagdollOnGround = false;
	bool bRagdollFaceUp = false;
	bool bPreRagdollURO = false;
	FVector LastRagdollVelocity = FVector::ZeroVector;
	FVector TargetRagdollLocation = FVector::ZeroVector;
	
	/////////////////////////////////
	/// Animation
	/////////////////////////////////
protected:
	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* K2_GetGetUpAnimation(bool bIsFaceUp) const;
	
	float TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData);
	float ForcePlayAnimMontage(const FAnimMontagePlayData& AnimMontageData);
	void ForceStopAnimMontage(UAnimMontage* AnimMontage);
private:
	void Internal_StopAllAnimMontages() const;
	float Internal_PlayMontage(const FAnimMontagePlayData& AnimMontagePlayData);
	
	/////////////////////////////////
	/// FORCEINLINE
	/////////////////////////////////
public:
	FORCEINLINE bool IsAlive() { return !UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Dead); }
};
