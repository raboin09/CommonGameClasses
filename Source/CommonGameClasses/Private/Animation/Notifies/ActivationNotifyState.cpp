// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/Notifies/ActivationNotifyState.h"
#include "ActorComponent/AbilityComponent.h"

UAbilityComponent* UActivationNotifyState::GetAbilityComponentFromOwner(const USkeletalMeshComponent* MeshComp) const
{
	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) {
		return nullptr;
	}
	
	return MeshComp->GetOwner()->FindComponentByClass<UAbilityComponent>();
}
