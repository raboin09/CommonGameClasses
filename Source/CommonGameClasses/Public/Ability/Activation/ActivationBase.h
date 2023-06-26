// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "Types/CommonAbilityTypes.h"
#include "API/Ability/ActivationMechanism.h"
#include "ActivationBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMECLASSES_API AActivationBase : public ACommonActor, public IActivationMechanism
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override  PURE_VIRTUAL(AActivationBase::Activate, )
	virtual void Deactivate() override PURE_VIRTUAL(AActivationBase::Deactivate, )

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	EMeshType MeshType = EMeshType::AbilityMesh;
	
	UPROPERTY()
	UMeshComponent* MeshComponentRef = nullptr;

private:
	void InitAssignMeshComponentRef();
};
