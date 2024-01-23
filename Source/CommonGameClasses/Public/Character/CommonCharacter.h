// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "Ability/CommonAbility.h"
#include "Animation/CommonAnimInstance.h"
#include "API/Taggable.h"
#include "Types/CommonTagTypes.h"
#include "CommonCharacter.generated.h"

class UMountManagerComponent;
struct FGameplayTag;
class IMountable;
class UHealthComponent;
class UGameplayTagComponent;
class UCommonCharacterMovementComponent;
class UEffectContainerComponent;
class UCharacterAnimationComponent;

UCLASS(Abstract, NotBlueprintable, AutoExpandCategories=("CUSTOM", "CUSTOM|Defaults"), PrioritizeCategories = "CUSTOM",
	AutoCollapseCategories=("Actor Tick", "Component Tick", "Tags", "Physics", "Asset User Data", "Collision", "Lighting", "Activation", "Variable", "Cooking", "Rendering", "Clothing"),
	HideCategories=("Replication", "Component Replication", "Skin Weights", "HLOD", "Path Tracing", "Mobile", "Navigation", "Virtual Texture"))
class COMMONGAMECLASSES_API ACommonCharacter : public AAlsCharacter, public ITaggable
{
	GENERATED_BODY()

public:	
	ACommonCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent)
	float GetMaxSpeedRatio() const;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload) override;
	UFUNCTION(BlueprintCallable)
	virtual void HandleDeath();
	UFUNCTION(BlueprintCallable)
	void SetMoveSpeedRatioIncrease(float Ratio);
	
	////////////////////////////////
	/// Common Events
	////////////////////////////////
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "COMMON|Character")
	void K2_HandleDeath();
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

	UPROPERTY(BlueprintReadOnly)
	UAbilityComponent* AbilityComponent;
	UPROPERTY(BlueprintReadOnly)
	UCharacterAnimationComponent* CharacterAnimationComponent;
	UPROPERTY(BlueprintReadOnly)
	UCommonCharacterMovementComponent* CommonCharacterMovementComponent;
	UPROPERTY()
	UEffectContainerComponent* EffectContainerComponent;
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;

	////////////////////////////////
	/// Knockbacks and Hit Reacts
	////////////////////////////////
public:
	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* K2_GetHitReactAnimation(const FGameplayTag& HitReactDirection) const;
	
private:
	UPROPERTY(Transient)
	FTimerHandle TimerHandle_HideWeapons;
	UPROPERTY(Transient)
	EHitReactType LastKnownHitReact;
	
	/////////////////////////////////
	/// Animation
	/////////////////////////////////
protected:
	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* K2_GetGetUpAnimation(bool bIsFaceUp) const;
	
	/////////////////////////////////
	/// FORCEINLINE
	/////////////////////////////////
public:
	FORCEINLINE bool IsAlive() { return !UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Dead); }
	FORCEINLINE UGameplayTagComponent* GetGameplayTagComponent() const { return GameplayTagComponent; }
	FORCEINLINE TWeakObjectPtr<UAnimInstance> GetAnimInstance() const { return AnimationInstance; }
};
