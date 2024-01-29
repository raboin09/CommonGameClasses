// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Interactable.h"
#include "Character/CommonCharacter.h"
#include "CommonAICharacter.generated.h"

class UInteractionComponent;
class UBotBehaviorComponent;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM"), PrioritizeCategories = "CUSTOM")
class COMMONGAMECLASSES_API ACommonAICharacter : public ACommonCharacter, public IInteractable
{
	GENERATED_BODY()
public:
	ACommonAICharacter(const FObjectInitializer& ObjectInitializer);
	
	//////////////////////////////////////
	// Interactable Overrides
	//////////////////////////////////////
	virtual void HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload) override;
	virtual void HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload) override;
	virtual void HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload) override;

	//////////////////////////////////////
	// ACommonCharacter Overrides
	//////////////////////////////////////sssd
	virtual void HandleDeath() override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload);

	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBotBehaviorComponent> BotBehaviorComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInteractionComponent> InteractionComponent;
};
