// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Types/CommonTagTypes.h"
#include "AbilityComponent.generated.h"

USTRUCT()
struct FAwaitingActivationDetails
{
	GENERATED_BODY()

	TWeakInterfacePtr<IActivationMechanism> MechanismAwaitingActivation = nullptr;
	int32 ActivationLevel = -1;
		
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void DestroyAbilities();
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void SetCurrentEquippedSlot(const FGameplayTag& NewEquippedSlot);
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void AddAbilityFromClassInSlot(TSoftClassPtr<AActor> AbilityClass, const FGameplayTag& SlotTag);

	UFUNCTION(BlueprintCallable, Category = "COMMON")
	FORCEINLINE void TryStartEquippedAbility() { TryStartAbilityInSlot(EquippedSlot); }
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	FORCEINLINE void TryStopEquippedAbility() { TryStopAbilityInSlot(EquippedSlot); }
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void TryStartAbilityInSlot(const FGameplayTag& SlotTag);
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	void TryStopAbilityInSlot(const FGameplayTag& SlotTag);

	void TryActivateAwaitingMechanism(bool bShouldActivate);
	
private:
	TWeakInterfacePtr<IAbility> Internal_SpawnAbilityFromClass(TSubclassOf<AActor> AbilityClass) const;
	void Internal_InitAndAttachAbilityToOwnerMesh(TWeakInterfacePtr<IAbility> AbilityToAttach) const;
	static void Internal_DestroyAbility(TWeakInterfacePtr<IAbility> AbilityToRemove);
	void Internal_AddAbilityInSlot(const FGameplayTag& SlotTag, TWeakInterfacePtr<IAbility> AbilityToAdd);
	void Internal_RemoveAbilityInSlot(const FGameplayTag& SlotTag);
	TWeakInterfacePtr<IAbility> Internal_FindAbility(const FGameplayTag& SlotTag) const;
	
	UPROPERTY()
	FAwaitingActivationDetails AwaitingActivationDetails;
	UPROPERTY()
	FGameplayTag EquippedSlot = CommonGameSlot::SlotMain;
	UPROPERTY(BlueprintAssignable)
	FNewAbilityAdded NewAbilityAdded;
	UPROPERTY(BlueprintAssignable)
	FAbilityRemoved AbilityRemoved;
	UPROPERTY(BlueprintAssignable)
	FNewAbilityEquipped AbilityEquipped;

	TMap<FGameplayTag, TWeakInterfacePtr<IAbility>> SlottedAbilities;
public:
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	FORCEINLINE TScriptInterface<IAbility> GetAbilityInSlot(const FGameplayTag& SlotTag) const { return Internal_FindAbility(SlotTag).ToScriptInterface(); }
	FORCEINLINE TWeakInterfacePtr<IAbility> GetAbilityInSlot_Weak(const FGameplayTag& SlotTag) const { return Internal_FindAbility(SlotTag); }
	
	UFUNCTION(BlueprintCallable, Category = "COMMON")
	FORCEINLINE bool HasAbilityInSlot(const FGameplayTag& SlotTag) const { return SlottedAbilities.Contains(SlotTag); }
	
	FORCEINLINE void ResetAwaitingActivationDetails() { AwaitingActivationDetails = FAwaitingActivationDetails(); }
	FORCEINLINE void SetMechanismAwaitingActivation(const FAwaitingActivationDetails& InDetails) { AwaitingActivationDetails = InDetails; }

	FORCEINLINE FNewAbilityEquipped& OnNewAbilityEquipped() { return AbilityEquipped; }
	FORCEINLINE FNewAbilityAdded& OnNewAbilityAdded() { return NewAbilityAdded; }
	FORCEINLINE FAbilityRemoved& OnAbilityRemoved() { return AbilityRemoved; }
	FORCEINLINE const FAwaitingActivationDetails& GetAwaitingActivationDetails() const { return AwaitingActivationDetails; }
};
