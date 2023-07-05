// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/Notifies/CommitAbilityNotifyState.h"
#include "Types/CommonTagTypes.h"

void UCommitAbilityNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	const UAbilityComponent* AbilityComponent = GetAbilityComponentFromOwner(MeshComp);
	if(!AbilityComponent)
	{
		return;
	}
	
	UGameplayTagComponent* AbilityTagComponent = AbilityComponent->GetAwaitingActivationDetails().AbilityTagComponent;
	if(!AbilityTagComponent){
		return;
	}
	UGameplayTagComponent::AddTagToActor(nullptr, TAG_ABILITY_COMMITTED, AbilityTagComponent);
}

void UCommitAbilityNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UAbilityComponent* AbilityComponent = GetAbilityComponentFromOwner(MeshComp);
	if(!AbilityComponent)
	{
		return;
	}
	
	UGameplayTagComponent* AbilityTagComponent = AbilityComponent->GetAwaitingActivationDetails().AbilityTagComponent;
	if(!AbilityTagComponent){
		return;
	}
	UGameplayTagComponent::RemoveTagFromActor(nullptr, TAG_ABILITY_COMMITTED, AbilityTagComponent);
	AbilityComponent->ResetAwaitingActivationDetails();
}
