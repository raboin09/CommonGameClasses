// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacter.h"
#include "AlsAnimationInstance.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Animation/CommonAnimInstance.h"
#include "API/Taggable.h"
#include "Types/CommonTagTypes.h"
#include "CommonCharacter.generated.h"

class UCommonAnimInstance;
class UMountManagerComponent;
struct FGameplayTag;
class IMountable;
class UHealthComponent;
class UGameplayTagComponent;
class UCommonCharacterMovementComponent;
class UEffectContainerComponent;
class UCharacterAnimationComponent;
class UAbilityComponent;
class UAlsAnimationInstance;

UCLASS(Abstract, NotBlueprintable, HideCategories=("Replication", "SkinWeights", "HLOD", "PathTracing", "Mobile", "Navigation",
	"VirtualTexture","ActorTick", "ComponentTick", "Tags", "Physics", "AssetUserData", "Collision", "Lighting", "Activation", "Variable", "Cooking", "Rendering", "Clothing",
	"Actor", "Pawn", "State"), AutoExpandCategories=("CUSTOM"))
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
	TMap<FGameplayTag, TSoftClassPtr<ACommonAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilityComponent> AbilityComponent;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterAnimationComponent> CharacterAnimationComponent;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCommonCharacterMovementComponent> CommonCharacterMovementComponent;
	UPROPERTY()
	TObjectPtr<UEffectContainerComponent> EffectContainerComponent;
	UPROPERTY()
	TObjectPtr<UGameplayTagComponent> GameplayTagComponent;

	////////////////////////////////
	/// Knockbacks and Hit Reacts
	////////////////////////////////
	
private:
	UPROPERTY(Transient)
	FTimerHandle TimerHandle_HideWeapons;
	UPROPERTY(Transient)
	EHitReactType LastKnownHitReact;
	
	/////////////////////////////////
	/// Animation
	/////////////////////////////////
public:
	UFUNCTION(BlueprintImplementableEvent)
	TSoftObjectPtr<UAnimMontage> K2_GetGetUpAnimation(bool bIsFaceUp) const;
	UFUNCTION(BlueprintImplementableEvent)
	TSoftObjectPtr<UAnimMontage> K2_GetHitReactAnimation(const FGameplayTag& HitReactDirection) const;
	
	/////////////////////////////////
	/// FORCEINLINE
	/////////////////////////////////
public:
	FORCEINLINE bool IsAlive() { return !UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Dead); }
	FORCEINLINE TWeakObjectPtr<UGameplayTagComponent> GetGameplayTagComponent() const { return GameplayTagComponent; }
	FORCEINLINE TWeakObjectPtr<UCommonAnimInstance> GetAnimInstance() const { return Cast<UCommonAnimInstance>(AnimationInstance); }
};
