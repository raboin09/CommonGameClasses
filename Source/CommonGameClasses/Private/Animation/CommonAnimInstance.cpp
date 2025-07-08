// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CommonAnimInstance.h"

#include "ActorComponent/CommonCharacterMovementComponent.h"
#include "Character/CommonCharacter.h"

void UCommonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Internal_AssignRequiredVars())
	{
		return;
	}
	const FCharacterGroundAnimInfo& GroundInfo = CommonCharacterMovementComponent->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

bool UCommonAnimInstance::Internal_AssignRequiredVars()
{
	if (CommonCharacter.IsValid() && CommonCharacterMovementComponent.IsValid())
	{
		return true;
	}

	if(!CommonCharacter.IsValid())
	{
		CommonCharacter = Cast<ACommonCharacter>(GetOwningActor());
		if(!CommonCharacter.IsValid())
		{
			return false;
		}
	}
	
	if(!CommonCharacterMovementComponent.IsValid())
	{
		CommonCharacterMovementComponent = CastChecked<UCommonCharacterMovementComponent>(CommonCharacter->GetCharacterMovement());
		if(!CommonCharacterMovementComponent.IsValid())
		{
			return false;
		}
	}
	return true;
}

bool UCommonAnimInstance::AnimOwnerHasGameplayTag(const FGameplayTag Tag) const
{
	return UGameplayTagComponent::ActorHasGameplayTag(GetOwningActor(), Tag);
}
