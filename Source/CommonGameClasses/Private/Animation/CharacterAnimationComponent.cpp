
#include "Animation/CharacterAnimationComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCharacterAnimTypes.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwnerCharacter = nullptr;
	OwningTagComponent = nullptr;
}

float UCharacterAnimationComponent::TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if(!OwnerCharacter)
	{
		return -1.f;
	}
	return OwnerCharacter->PlayAnimMontage(AnimMontageData.MontageToPlay);
}

void UCharacterAnimationComponent::SetAnimationOverlay(const FGameplayTag& NewOverlay)
{
	if(!OwnerCharacter)
	{
		return;
	}
	OwnerCharacter->SetOverlayMode(NewOverlay);
}

void UCharacterAnimationComponent::StartRagdolling()
{
	
}

void UCharacterAnimationComponent::StopRagdolling()
{
	
}

void UCharacterAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACommonCharacter>(GetOwner());
}
