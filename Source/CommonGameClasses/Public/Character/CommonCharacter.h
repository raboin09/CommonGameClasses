// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/EnergyComponent.h"
#include "API/Taggable.h"
#include "Types/CommonTypes.h"
#include "GameFramework/Character.h"
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
	ACommonCharacter();

	FORCEINLINE TScriptInterface<IMountable> GetCurrentMount() const { return CurrentMount; }
	FORCEINLINE UCommonCharacterMovementComponent* GetMyMovementComponent() const { return MyCharacterMovementComponent; }
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void HandleTagAdded(const FGameplayTagAddedEventPayload TagAddedEventPayload) override;
	virtual void HandleTagRemoved(const FGameplayTagRemovedEventPayload TagRemovedEventPayload) override;
	
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

	UPROPERTY()
	UEnergyComponent* ManaComponent;
	UPROPERTY()
	UEffectContainerComponent* EffectContainerComponent;
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;
	UPROPERTY()
	TScriptInterface<IMountable> CurrentMount;
	UPROPERTY()
	UCommonCharacterMovementComponent* MyCharacterMovementComponent;
};
