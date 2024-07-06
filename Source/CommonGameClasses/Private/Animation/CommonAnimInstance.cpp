// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CommonAnimInstance.h"

#include "ActorComponent/CommonCharacterMovementComponent.h"
#include "Character/CommonCharacter.h"

void UCommonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const ACommonCharacter* Character = Cast<ACommonCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UCommonCharacterMovementComponent* CharMoveComp = CastChecked<UCommonCharacterMovementComponent>(Character->GetCharacterMovement());
	const FCharacterGroundAnimInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
	bGameplayTag_IsReady = UGameplayTagComponent::ActorHasGameplayTag(Character, CommonGameState::Ready);
	bGameplayTag_IsDashing = UGameplayTagComponent::ActorHasGameplayTag(Character, CommonGameState::Dashing);
	bGameplayTag_IsFiring = UGameplayTagComponent::ActorHasGameplayTag(Character, CommonGameState::Firing);
	bGameplayTag_IsAiming = UGameplayTagComponent::ActorHasGameplayTag(Character, CommonGameState::Aiming);
	bGameplayTag_IsMelee = UGameplayTagComponent::ActorHasGameplayTag(Character, CommonGameState::MeleeEquipped);
	bGameplayTag_IsSprinting = UGameplayTagComponent::ActorHasGameplayTag(Character, CommonGameState::Sprinting);
}
