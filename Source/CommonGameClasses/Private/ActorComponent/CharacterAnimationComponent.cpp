
#include "ActorComponent/CharacterAnimationComponent.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UCharacterAnimationComponent::TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	return 0.f;
}
