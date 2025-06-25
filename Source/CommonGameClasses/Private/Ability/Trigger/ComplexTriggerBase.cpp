// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Trigger/ComplexTriggerBase.h"


int32 UComplexTriggerBase::K2N_GetActivationLevel_Implementation() const
{
	return 1;
}

FGameplayTag UComplexTriggerBase::K2N_GetActivationDescriptorTag_Implementation() const
{
	return FGameplayTag();
}
