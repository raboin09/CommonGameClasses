// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Interactable.h"
#include "Character/CommonCharacter.h"
#include "CommonAICharacter.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonAICharacter : public ACommonCharacter, public IInteractable
{
	GENERATED_BODY()
public:
	ACommonAICharacter();
	FORCEINLINE virtual UBehaviorTree* GetDefaultBehavior() const { return DefaultBehaviorTree; }
	
	//////////////////////////////////////
	// Interactable Overrides
	//////////////////////////////////////
	virtual void SwitchOutlineOnMesh(bool bShouldOutline) override;
	virtual void InitiateInteractionWithActor(AActor* InstigatingActor, bool bStartingInteraction = true) override;
	virtual void HandleInteractionStarted(const FInteractionEventPayload& InteractionEventPayload) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleMeshOutlining(bool bIsOutlining);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionStarted(AActor* InstigatingActor);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleInteractionInitiated(AActor* InstigatingActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInteractionComponent* InteractionComponent;
	UPROPERTY(EditAnywhere, Category="COMMON|Defaults")
	UBehaviorTree* DefaultBehaviorTree;
};
