﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CommonGameSlot
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SlotMain)
}

namespace CommonGameActorType
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActorTypeCover)	
}

namespace CommonGameAbilityEvent
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(OnCooldown)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Activated)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RequestingStart)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Committed)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboWindowEnabled)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboActivated)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active)
}

namespace CommonGameState
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aiming)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stunned)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Immovable)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CannotSprint)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aloof)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Loading)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArmorPiercing)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dead)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Firing)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ready)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ragdoll)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InCover)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CannotMount)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InCombat)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoverLeftPeek)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoverLeftEdge)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoverRightPeek)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoverRightEdge)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoverRolledOut)
}

namespace CommonGameAnimation
{
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unarmed)
	
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactLeft)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactFront)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactRight)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactBack)
	COMMONGAMECLASSES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactChainsaw)	
}

#define TAG_NAME_COVERMESH FName("CoverMesh")