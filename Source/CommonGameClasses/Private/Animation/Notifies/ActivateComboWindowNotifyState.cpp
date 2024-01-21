// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ActivateComboWindowNotifyState.h"

void UActivateComboWindowNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UAbilityComponent* AbilityComponent = GetAbilityComponentFromOwner(MeshComp);
	if(!AbilityComponent)
	{
		return;
	}
	AActor* CastedAbility = Cast<AActor>(AbilityComponent->GetAbilityInSlot(AbilitySlotTag).GetObject());
	if(!CastedAbility)
	{
		return;
	}
	UGameplayTagComponent::RemoveTagFromActor(CastedAbility, CommonGameAbilityEvent::ComboActivated);
	UGameplayTagComponent::AddTagToActor(CastedAbility, CommonGameAbilityEvent::ComboWindowEnabled);
}

void UActivateComboWindowNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UAbilityComponent* AbilityComponent = GetAbilityComponentFromOwner(MeshComp);
	if(!AbilityComponent)
	{
		return;
	}
	AActor* CastedAbility = Cast<AActor>(AbilityComponent->GetAbilityInSlot(AbilitySlotTag).GetObject());
	if(!CastedAbility)
	{
		return;
	}
	UGameplayTagComponent::RemoveTagFromActor(CastedAbility, CommonGameAbilityEvent::ComboWindowEnabled);
}