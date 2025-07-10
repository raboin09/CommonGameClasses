// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/ActivationMechanism.h"
#include "BaseActivation.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API UBaseActivation : public UObject, public IActivationMechanism
{
	GENERATED_BODY()

public:
	virtual void InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent) override;
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual void Deactivate() override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_OnActivation();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_OnDeactivation();

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(MustImplement = "/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> OwnerApplyEffectsOnActivation;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(MustImplement = "/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> OwnerApplyEffectsOnDeactivation;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UMeshComponent> MeshComponentRef = nullptr; 
};
