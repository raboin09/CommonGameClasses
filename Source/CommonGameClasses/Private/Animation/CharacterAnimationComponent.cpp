
#include "Animation/CharacterAnimationComponent.h"

#include "GameFramework/Character.h"
#include "Types/CommonCharacterAnimTypes.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UCharacterAnimationComponent::TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if(!OwnerCharacter)
	{
		return -1.f;
	}
	return OwnerCharacter->PlayAnimMontage(AnimMontageData.MontageToPlay);
}

void UCharacterAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACommonCharacter>(GetOwner());
	if(!OwnerCharacter)
	{
		return;
	}
	
	OwningTagComponent = OwnerCharacter->FindComponentByClass<UGameplayTagComponent>();
	if(OwningTagComponent)
	{
		OwningTagComponent->OnGameplayTagAdded().AddUObject(this, &UCharacterAnimationComponent::HandleTagAdded);
	}
	
	if(!OwnerCharacter->GetMesh())
	{
		return;
	}
	OwnerAnimInstance = Cast<UCommonAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
}

void UCharacterAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(OwnerAnimInstance)
	{
		OwnerAnimInstance->RotationMethod = RotationMethod;
	}
}

void UCharacterAnimationComponent::HandleTagAdded(const FGameplayTagAddedEventPayload& GameplayTagAddedEventPayload)
{
	if(!OwnerAnimInstance)
	{
		return;
	}
	
	if(GameplayTagAddedEventPayload.AddedTag.MatchesTag(TAG_ANIM_BASEPOSE))
	{
		OwnerAnimInstance->BasePose = GameplayTagAddedEventPayload.AddedTag;
	} else if(GameplayTagAddedEventPayload.AddedTag.MatchesTag(TAG_ANIM_OVERLAYPOSE))
	{
		OwnerAnimInstance->OverlayPose = GameplayTagAddedEventPayload.AddedTag;
	}
}
