// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	void SwitchOutlineOnAllMeshes(bool bShouldOutline);
	
protected:
	virtual void BeginPlay() override;

private:
	void Internal_AddOwnerMeshesToArray();
	
	UPROPERTY()
	TArray<UMeshComponent*> OwnerMeshes;
};
