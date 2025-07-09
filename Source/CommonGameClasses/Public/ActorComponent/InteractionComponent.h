// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "..\Types\CommonInteractTypes.h"
#include "Types/CommonEventDeclarations.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Variable", "Sockets", "Tags", "ComponentReplication", "Component Replication", "Cooking", "Replication", "AssetUserData", "Asset User Data", "Navigation"))
class COMMONGAMECLASSES_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	void SwitchOutlineOnAllMeshes(bool bShouldOutline);
	/**
	 * Initiates an interaction process with the interaction component.
	 * This function handles the starting or stopping of the interaction depending on the provided inputs.
	 *
	 * @param InstigatingActor The actor initiating the interaction.
	 * @param bStartingInteraction Whether the interaction is starting (true) or stopping (false).
	 */
	void InitiateInteraction(AActor* InstigatingActor, bool bStartingInteraction);
	FORCEINLINE FVector GetOwnerLocation() const { return GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="COMMON|Interact")
	EAffiliation Affiliation = EAffiliation::Neutral;
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Determines whether interactions with this component should be initiated instantly.
	 * When set to true, interactions occur without requiring any hold duration or delay.
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact")
	bool bInteractInstantly = true;
	/**
	 * Specifies the duration required to complete an interaction with this component.
	 * This value is ignored when interactions are set to occur instantly (bInteractInstantly is true).
	 */
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact", meta=(EditCondition="!bInteractInstantly", EditConditionHides))
	float InteractTime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact")
	FString InteractionText = "Placeholder";
	
private:
	void InitEventListeners();
	void Internal_AddOwnerMeshesToArray();
	void Internal_SetupInteractionTimeline();
	
	UFUNCTION()
	void Internal_InteractionTick();
	UFUNCTION()
	void Internal_InteractionFinished();
	
	UPROPERTY()
	TArray<TWeakObjectPtr<UMeshComponent>> OwnerMeshes;
	UPROPERTY()
	TWeakObjectPtr<AActor> CachedInstigatingActor;
	UPROPERTY()
	FTimeline Timeline;

	FInteractionOutlinedEvent InteractionOutlinedEvent;
	FInteractionStartedEvent InteractionStartedEvent;
	FInteractionInitiatedEvent InteractionInitiatedEvent;
};
