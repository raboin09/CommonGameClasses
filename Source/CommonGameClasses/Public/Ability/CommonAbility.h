﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/Ability.h"
#include "Types/CommonEventDeclarations.h"
#include "CommonAbility.generated.h"

class IResourceContainer;
class USphereComponent;
enum class EResourceContainerLocation : uint8;

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
	
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Ability", meta = (ClampMin="0"))
	float CooldownDuration = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Ability", meta = (ClampMin="0"))
	int32 ResourceCost = 0;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Ability", meta = (EditCondition = "ResourceCost > 0", EditConditionHides))
	EResourceContainerLocation ResourceContainerLocation;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Ability", meta = (MustImplement = "/Script/CommonGameClasses.ResourceContainer", EditCondition = "ResourceCost > 0 && (ResourceContainerLocation == EResourceContainerLocation::InstigatorComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerControllerComponent || ResourceContainerLocation == EResourceContainerLocation::AbilityComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerStateComponent)", EditConditionHides))
	TSubclassOf<UActorComponent> ResourceContainerClass;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability", meta = (MustImplement = "/Script/CommonGameClasses.TriggerMechanism"))
	TSubclassOf<AActor> TriggerMechanismClass;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability", meta = (MustImplement = "/Script/CommonGameClasses.ActivationMechanism"))
	TSubclassOf<AActor> ActivationMechanismClass;
	
	UPROPERTY(VisibleDefaultsOnly, Category="CUSTOM|Ability")
	USphereComponent* AbilityRoot;

private:	
	void SetTriggerMechanism(const TSubclassOf<AActor> InTriggerClass);
	void SetCooldownMechanism();
	void SetResourceContainerObject();
	void SetActivationMechanism(const TSubclassOf<AActor> InActivationClass);

	void Internal_SetResourceContainerToObject(UObject* ContainerObject);
	void Internal_SetResourceContainerToComponent(const AActor* PotentialActor);
	bool Internal_StartNormalAbility() const;
	AActor* Internal_CreateNewMechanism(const TSubclassOf<AActor> InMechanismClass, const UClass* InterfaceClass);
	void Internal_BindMechanismEventsToAbility();

	UFUNCTION()
	void HandleAbilityActivationEvent(const FAbilityActivationEventPayload& AbilityActivationEventPayload) const;
	UFUNCTION()
	void HandleAbilityDeactivationEvent(const FAbilityDeactivationEventPayload& AbilityDeactivationEventPayload) const;
	UFUNCTION()
	void HandleTriggerActivationEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload) const;
	UFUNCTION()
	void HandleTriggerDeactivationEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload) const;
	UFUNCTION()
	void HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent) const;
	UFUNCTION()
	void HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent) const;

	UPROPERTY()
	TScriptInterface<ICooldownMechanism> CooldownMechanism;
	UPROPERTY()
	TScriptInterface<IResourceContainer> ResourceContainer;
	UPROPERTY()
	TScriptInterface<ITriggerMechanism> TriggerMechanism;
	UPROPERTY()
	TScriptInterface<IActivationMechanism> ActivationMechanism;
	UPROPERTY()
	AActor* OwningActor;
}; 