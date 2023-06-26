// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ActivateAbilityNotifyState.h"

void UActivateAbilityNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UAbilityComponent* AbilityComponent = GetAbilityComponentFromOwner(MeshComp);
	if(!AbilityComponent)
	{
		return;
	}
	AbilityComponent->TryActivateAwaitingMechanism(true);
}

void UActivateAbilityNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UAbilityComponent* AbilityComponent = GetAbilityComponentFromOwner(MeshComp);
	if(!AbilityComponent)
	{
		return;
	}
	AbilityComponent->TryActivateAwaitingMechanism(false);
}