// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/Ability.h"
#include "Types/EventDeclarations.h"
#include "CommonAbility.generated.h"

UCLASS(Abstract, Blueprintable)
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
	
	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Ability")
	float CooldownDuration;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability", meta = (MustImplement = "/Script/CommonGameClasses.CostMechanism"))
	TSubclassOf<AActor> CostMechanismClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability", meta = (MustImplement = "/Script/CommonGameClasses.TriggerMechanism"))
	TSubclassOf<AActor> TriggerMechanismClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Ability", meta = (MustImplement = "/Script/CommonGameClasses.ActivationMechanism"))
	TSubclassOf<AActor> ActivationMechanismClass;

	// If the Activation occurs due to an external event (like Montage notifies) instead of from a Trigger
	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Ability")
	bool bActivatesOnExternalEvent = false;
	
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
	void HandleTriggerActivationEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload) const;
	UFUNCTION()
	void HandleTriggerDeactivationEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload);
	UFUNCTION()
	void HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent);
	UFUNCTION()
	void HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent);

	UPROPERTY()
	TScriptInterface<class ICooldownMechanism> CooldownMechanism;
	UPROPERTY()
	TScriptInterface<class ICostMechanism> CostMechanism;
	UPROPERTY()
	TScriptInterface<ITriggerMechanism> TriggerMechanism;
	UPROPERTY()
	TScriptInterface<IActivationMechanism> ActivationMechanism;
};
