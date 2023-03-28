// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "API/Interactable.h"
#include "Types/EventDeclarations.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	void SwitchOutlineOnAllMeshes(bool bShouldOutline);
	void StartInteraction(AActor* InstigatingActor, bool bStartingInteraction);
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Interact")
	bool bInteractInstantly = true;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Interact", meta=(EditCondition="!bInteractInstantly", EditConditionHides))
	float InteractTime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Interact")
	FString InteractionText = "Placeholder";
	
private:
	void Internal_AddOwnerMeshesToArray();
	void Internal_SetupInteractionTimeline();
	
	UFUNCTION()
	void Internal_InteractionTick();
	UFUNCTION()
	void Internal_InteractionFinished();
	
	UPROPERTY()
	TArray<UMeshComponent*> OwnerMeshes;
	UPROPERTY()
	AActor* CachedInstigatingActor;
	UPROPERTY()
	FTimeline Timeline;
	FInteractionEvent InteractionEvent;
};
