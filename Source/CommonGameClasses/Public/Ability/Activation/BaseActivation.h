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
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override PURE_VIRTUAL(UBaseActivation::Activate, )
	virtual void Deactivate() override PURE_VIRTUAL(UBaseActivation::Deactivate, )

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_OnActivation();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_OnDeactivation();
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UMeshComponent> MeshComponentRef = nullptr; 
};
