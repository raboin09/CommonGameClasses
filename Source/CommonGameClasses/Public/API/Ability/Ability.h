// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Ability.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual bool TryStartAbility() PURE_VIRTUAL(IAbility::TryStartAbility, return false;)
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual bool TryEndAbility() PURE_VIRTUAL(IAbility::TryEndAbility, return false;)
	virtual void InitAbility(UMeshComponent* OwnerMeshComponent) PURE_VIRTUAL(IAbility::InitAbility, )
	virtual UMeshComponent* GetAbilityMesh() const PURE_VIRTUAL(IAbility::GetAbilityMesh, return nullptr; )
};
