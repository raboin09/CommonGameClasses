// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CommonCharacterMovementComponent.h"

void UCommonCharacterMovementComponent::SetWalkSpeedRatio(float Ratio)
{
	WalkSpeedModifierRatio = Ratio;
	RefreshMaxWalkSpeed();
}
