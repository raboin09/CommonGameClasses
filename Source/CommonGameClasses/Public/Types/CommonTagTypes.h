// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CommonGameCore
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PlayerCharacter)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AICharacter)
}

namespace CommonGameSlot   
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SlotMain)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SlotAlt)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SlotJump)
}

namespace CommonGameTriggerEvent
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TriggerActivationCount)
}

namespace CommonGameAbilityEvent
{
	// Ability is busy equipping
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Equipping)
	// The cooldown mechanism is ticking 
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnCooldown)
	// The activation mechanism is activated
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Activated)
	// The player has requested the ability to start (though it may not due to trigger conditions)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RequestingStart)
	// If the ability should re-run after an event (e.g. combo has been triggered, after equipping)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AutoStartAbility)
	// The montage trigger mechanism has committed to the playing montage (and it can't be canceled)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Committed)
	// The montage trigger mechanism is able to chain the next combo 
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboWindowEnabled)
	// The player triggered a combo during a valid window
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboActivated)
	// The ability/trigger is active (basically, is this ability/trigger doing SOMETHING other than being idle)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active)
}

namespace CommonGameState
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active)

	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CannotMove)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Immovable)

	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CannotMount)

	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ragdoll)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dead)
}

namespace CommonGameAnimation
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unarmed)
	
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactLeft)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactFront)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactRight)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactBack)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactSpecial1)
}
