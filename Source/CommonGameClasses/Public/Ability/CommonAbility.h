// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/Ability.h"
#include "Types/EventDeclarations.h"
#include "CommonAbility.generated.h"

UCLASS()
class COMMONGAMECLASSES_API ACommonAbility : public ACommonActor, public IAbility
{
	GENERATED_BODY()
	
public:
	ACommonAbility();
	virtual bool TryStartAbility() override;
	virtual bool TryEndAbility() override;
	virtual void CommitAbility() override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "COMMON|Ability")
	class UCooldownMechanismImpl* Cooldown;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability", meta = (MustImplement = "CostMechanism"))
	TSubclassOf<AActor> CostMechanismClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability", meta = (MustImplement = "TriggerMechanism"))
	TSubclassOf<AActor> TriggerMechanismClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability", meta = (MustImplement = "ActivationMechanism"))
	TSubclassOf<AActor> ActivationMechanismClass;
	
	UPROPERTY(VisibleDefaultsOnly, Category="COMMON|Ability")
	class USphereComponent* AbilityRoot;

private:	
	void SetTriggerMechanism(const TSubclassOf<AActor> InTriggerClass);
	void SetCooldownMechanism();
	void SetCostMechanism(const TSubclassOf<AActor> InCostClass);
	void SetActivationMechanism(const TSubclassOf<AActor> InActivationClass);
	
	bool Internal_StartNormalAbility();
	AActor* Internal_CreateNewMechanism(const TSubclassOf<AActor> InMechanismClass, const UClass* InterfaceClass);
	void Internal_BindMechanismEventsToAbility();

	UFUNCTION()
	void HandleAbilityActivationEvent(const FAbilityActivationEventPayload& AbilityActivationEventPayload);
	UFUNCTION()
	void HandleAbilityDeactivationEvent(const FAbilityDeactivationEventPayload& AbilityDeactivationEventPayload);
	UFUNCTION()
	void HandleTriggerActivationEvent(const FTriggerActivatedEventPayload& AbilityTriggeredEventPayload) const;
	UFUNCTION()
	void HandleTriggerDeactivationEvent(const FTriggerDeactivatedEventPayload& AbilityTriggeredEventPayload);
	UFUNCTION()
	void HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent);
	UFUNCTION()
	void HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent);
	
	TScriptInterface<class ICooldownMechanism> CooldownMechanism;
	TScriptInterface<class ICostMechanism> CostMechanism;
	TScriptInterface<class ITriggerMechanism> TriggerMechanism;
	TScriptInterface<class IActivationMechanism> ActivationMechanism;
};
