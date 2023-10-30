
#include "Animation/CharacterAnimationComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
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
	// TODO Refactor anim
	UKismetSystemLibrary::PrintString(this,"Make ALS play anim montage, MontageTrigger::71");
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
}