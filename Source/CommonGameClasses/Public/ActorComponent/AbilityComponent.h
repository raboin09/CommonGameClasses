// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/Ability.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "API/Ability/ActivationMechanism.h"
#include "Types/CommonTagTypes.h"
#include "AbilityComponent.generated.h"

USTRUCT()
struct FAwaitingActivationDetails
{
	GENERATED_BODY()

	UPROPERTY()
	UGameplayTagComponent* AbilityTagComponent = nullptr;
	UPROPERTY()
	TScriptInterface<IActivationMechanism> MechanismAwaitingActivation = nullptr;
	UPROPERTY()
	int32 ActivationLevel = -1;
		
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void DestroyAbilities();
	
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void AddAbilityFromClassInSlot(TSubclassOf<AActor> AbilityClass, const FGameplayTag& SlotTag);
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void TryStartAbilityInSlot(const FGameplayTag& SlotTag);
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void TryStopAbilityInSlot(const FGameplayTag& SlotTag);

	void TryActivateAwaitingMechanism(bool bShouldActivate);
	
protected:
	virtual void BeginPlay() override;

private:
	TScriptInterface<IAbility> Internal_FindAbility(const FGameplayTag& SlotTag);
	
	UPROPERTY()
	TMap<FGameplayTag, TScriptInterface<IAbility>> SlottedAbilities;
	UPROPERTY()
	FAwaitingActivationDetails AwaitingActivationDetails;

	UPROPERTY(BlueprintAssignable)
	FNewAbilityAdded NewAbilityAdded;
	UPROPERTY(BlueprintAssignable)
	FAbilityRemoved AbilityRemoved;

public:
	FORCEINLINE FNewAbilityAdded& OnNewAbilityAdded() { return NewAbilityAdded; }
	FORCEINLINE FAbilityRemoved& OnAbilityRemoved() { return AbilityRemoved; }
	FORCEINLINE const FAwaitingActivationDetails& GetAwaitingActivationDetails() const { return AwaitingActivationDetails; }
	FORCEINLINE void ResetAwaitingActivationDetails() { AwaitingActivationDetails = FAwaitingActivationDetails(); }
	FORCEINLINE void SetMechanismAwaitingActivation(const FAwaitingActivationDetails& InDetails) { AwaitingActivationDetails = InDetails; }
};
