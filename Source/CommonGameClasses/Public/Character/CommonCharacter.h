// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/CommonAbility.h"
#include "ActorComponent/AbilityComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "API/Taggable.h"
#include "GameFramework/Character.h"
#include "Types/CommonTagTypes.h"
#include "CommonCharacter.generated.h"

struct FGameplayTag;
class IMountable;
class UHealthComponent;
class UGameplayTagComponent;
class UCommonCharacterMovementComponent;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API ACommonCharacter : public ACharacter, public ITaggable
{
	GENERATED_BODY()

public:
	ACommonCharacter(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE TScriptInterface<IMountable> GetCurrentMount() const { return CurrentMount; }
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload) override;

	UFUNCTION(BlueprintCallable)
	void TEST_FIRE_WEAPON()
	{
		AbilityComponent->TryStartAbilityInSlot(TAG_SLOT_MAIN);
	}
	
	UFUNCTION(BlueprintCallable)
	void TEST_STOP_FIRE_WEAPON()
	{
		AbilityComponent->TryStopAbilityInSlot(TAG_SLOT_MAIN);
	}
	
	////////////////////////////////
	/// Common Events
	////////////////////////////////
	UFUNCTION(BlueprintNativeEvent, Category = "COMMON|Character")
	void K2_OnDeath();
	virtual void K2_OnDeath_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Defaults")
	TMap<FGameplayTag, TSubclassOf<ACommonAbility>> DefaultAbilities;

	UPROPERTY()
	UAbilityComponent* AbilityComponent;
	UPROPERTY()
	UEffectContainerComponent* EffectContainerComponent;
	UPROPERTY()
	UCharacterAnimationComponent* CharacterAnimationComponent;
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;
	UPROPERTY()
	TScriptInterface<IMountable> CurrentMount;
};
