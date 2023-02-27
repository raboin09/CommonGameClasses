// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonCharacter.h"


// Sets default values
ACommonCharacter::ACommonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACommonCharacter::HandleTagChanged(const FGameplayTag& ChangedTag, bool bAdded)
{
	if(bAdded)
	{
		K2_HandleTagAdded(ChangedTag);
	}
	else
	{
		K2_HandleTagRemoved(ChangedTag);
	}
}

void ACommonCharacter::K2_OnDeath_Implementation()
{
	// Events that should occur on death but before destruction
}

void ACommonCharacter::K2_HandleTagRemoved_Implementation(const FGameplayTag& AddedTag)
{
	// Events that should trigger if a tag is removed
}

void ACommonCharacter::K2_HandleTagAdded_Implementation(const FGameplayTag& AddedTag)
{
	// Events that should trigger if a tag is added
}