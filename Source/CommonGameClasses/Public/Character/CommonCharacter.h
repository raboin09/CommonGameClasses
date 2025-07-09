// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Animation/CommonAnimInstance.h"
#include "API/Taggable.h"
#include "GameFramework/Character.h"
#include "Types/CommonTagTypes.h"
#include "CommonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
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
class UThirdPersonCameraComponent;

UCLASS(Abstract, NotBlueprintable, AutoExpandCategories=("COMMON"), PrioritizeCategories = ("COMMON"), HideCategories=("Replication", "Rendering", "Collision", "Actor", "HLOD", "Physics", "WorldPartition", "Events", "LevelInstance", "Cooking", "DataLayers", "Level Instance", "World Partition"))
class COMMONGAMECLASSES_API ACommonCharacter : public ACharacter, public ITaggable
{
	GENERATED_BODY()

public:
	ACommonCharacter(const FObjectInitializer& ObjectInitializer);
	
	// Public for resetting mesh props after ragdoll
	void InitCharacterMeshDefaults() const;
	void InitCapsuleDefaults() const;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload) override;
	UFUNCTION(BlueprintCallable)
	virtual void HandleDeath();

	void InitTopDownMovementDefaults() const;
	
	////////////////////////////////
	/// Common Events
	////////////////////////////////
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleDeath();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload);
	
	////////////////////////////////
	/// Common Variables
	////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COMMON|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ShowInnerProperties))
	TObjectPtr<UAbilityComponent> AbilityComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ShowInnerProperties))
	TObjectPtr<UCharacterAnimationComponent> CharacterAnimationComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UEffectContainerComponent> EffectContainerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(ShowInnerProperties))
	TObjectPtr<UGameplayTagComponent> GameplayTagComponent;

	////////////////////////////////
	/// Knockbacks and Hit Reacts
	////////////////////////////////
	
private:	
	/////////////////////////////////
	/// Animation
	/////////////////////////////////
public:
	UFUNCTION(BlueprintImplementableEvent)
	TSoftObjectPtr<UAnimMontage> BPI_GetGetUpAnimation(bool bIsFaceUp) const;
	UFUNCTION(BlueprintImplementableEvent)
	TSoftObjectPtr<UAnimMontage> BPI_GetHitReactAnimation(const FGameplayTag& HitReactDirection) const;
	
	/////////////////////////////////
	/// FORCEINLINE
	/////////////////////////////////
public:
	FORCEINLINE bool IsAlive() { return !UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Dead); }
	FORCEINLINE TObjectPtr<UGameplayTagComponent> GetGameplayTagComponent() const { return GameplayTagComponent; }
	FORCEINLINE TObjectPtr<UCommonAnimInstance> GetAnimInstance() const { return Cast<UCommonAnimInstance>(GetMesh()->GetAnimInstance()); }
};
