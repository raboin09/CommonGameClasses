// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "UObject/Interface.h"
#include "Ability.generated.h"

class UCooldownMechanismImpl;
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
	virtual void EquipAbility() PURE_VIRTUAL(IAbility::EquipAbility, )
	virtual void UnEquipAbility() PURE_VIRTUAL(IAbility::UnEquipAbility, )
	
	virtual bool TryStartAbility() PURE_VIRTUAL(IAbility::TryStartAbility, return false;)
	virtual bool TryEndAbility() PURE_VIRTUAL(IAbility::TryEndAbility, return false;)

	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual UObject* GetTriggerMechanismObject() const PURE_VIRTUAL(IAbility::GetTriggerMechanismObject, return nullptr;)
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual UObject* GetActivationMechanismObject() const PURE_VIRTUAL(IAbility::GetActivationMechanismObject, return nullptr;)
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual UCooldownMechanismImpl* GetCooldownMechanismObject() const PURE_VIRTUAL(IAbility::GetCooldownMechanismObject, return nullptr;)
	
	virtual void InitAbility(UMeshComponent* OwnerMeshComponent) PURE_VIRTUAL(IAbility::InitAbility, )
	virtual void DestroyAbility() PURE_VIRTUAL(IAbility::DestroyAbility, )

	// If this is equipped, the PlayerController may need this to know if it should outline an interaction Actor or not
	virtual float GetAbilityOutlineRange() const PURE_VIRTUAL(IAbility::GetAbilityOutlineRange, return -1.f; )
};
