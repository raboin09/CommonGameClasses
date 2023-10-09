#include "Animation/ThirdPersonAnimationComponent.h"
#include "Animation/ThirdPersonAnimInstance.h"

void UThirdPersonAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(ThirdPersonAnimInstance)
	{
		ThirdPersonAnimInstance->AimOffsetType = AimOffsetType;
		ThirdPersonAnimInstance->AimOffsetBehavior = AimOffsetBehavior;
		ThirdPersonAnimInstance->RawAimOffset = AimOffset;
		ThirdPersonAnimInstance->LookAtLocation = LookAtLocation;
		ThirdPersonAnimInstance->AimClamp = AimClamp;
	}
}

void UThirdPersonAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	ThirdPersonAnimInstance = Cast<UThirdPersonAnimInstance>(OwnerAnimInstance);
}
