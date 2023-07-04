// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonAbilityTypes.h"
#include "API/Ability/ActivationMechanism.h"
#include "BaseActivation.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API UBaseActivation : public UObject, public IActivationMechanism
{
	GENERATED_BODY()

public:
	virtual void InitActivationMechanism() override;
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override  PURE_VIRTUAL(AActivationBase::Activate, )
	virtual void Deactivate() override PURE_VIRTUAL(AActivationBase::Deactivate, )

protected:
	UPROPERTY(EditDefaultsOnly, Category="Activation")
	EMeshType MeshType = EMeshType::AbilityMesh;
	
	UPROPERTY()
	UMeshComponent* MeshComponentRef = nullptr;

private:
	void InitAssignMeshComponentRef();
};
