// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
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
	FORCEINLINE FAbilityFinishedEquip& OnAbilityFinishedEquip() { return AbilityFinishedEquip; }
	
	virtual void EquipAbility() PURE_VIRTUAL(IAbility::EquipAbility, )
	virtual void UnEquipAbility() PURE_VIRTUAL(IAbility::UnEquipAbility, )
	
	virtual bool TryStartAbility() PURE_VIRTUAL(IAbility::TryStartAbility, return false;)
	virtual bool TryEndAbility() PURE_VIRTUAL(IAbility::TryEndAbility, return false;)
	
	virtual void InitAbility(UMeshComponent* OwnerMeshComponent) PURE_VIRTUAL(IAbility::InitAbility, )
	virtual void DestroyAbility() PURE_VIRTUAL(IAbility::DestroyAbility, )

protected:
	FAbilityFinishedEquip AbilityFinishedEquip;
};
