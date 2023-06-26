
#include "ActorComponent/CharacterAnimationComponent.h"

#include "GameFramework/Character.h"
#include "Types/CommonCharacterAnimTypes.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UCharacterAnimationComponent::TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	ACharacter* Own = Cast<ACharacter>(GetOwner());
	Own->PlayAnimMontage(AnimMontageData.MontageToPlay);
	return 0.f;
}
