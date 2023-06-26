// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/CommitAbilityNotify.h"

#include "Types/CommonTagTypes.h"


void UCommitAbilityNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) {
		return;
	}
    
	const UAbilityComponent* AbilityComponent = MeshComp->GetOwner()->FindComponentByClass<UAbilityComponent>();
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
