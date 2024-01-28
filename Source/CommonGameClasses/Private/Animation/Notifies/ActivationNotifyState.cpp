// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/Notifies/ActivationNotifyState.h"
#include "ActorComponent/AbilityComponent.h"

TObjectPtr<UAbilityComponent> UActivationNotifyState::GetAbilityComponentFromOwner(const TObjectPtr<USkeletalMeshComponent> MeshComp) const
{
	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) {
		return nullptr;
	}
	
	return MeshComp->GetOwner()->FindComponentByClass<UAbilityComponent>();
}
