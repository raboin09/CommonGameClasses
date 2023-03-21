// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define TAG_ABILITY_ON_COOLDOWN FGameplayTag::RequestGameplayTag("AbilityEvent.OnCooldown")
#define TAG_ABILITY_ACTIVATED FGameplayTag::RequestGameplayTag("AbilityEvent.Activated")
#define TAG_ABILITY_REQUESTING_START FGameplayTag::RequestGameplayTag("AbilityEvent.RequestingStart")
#define TAG_ABILITY_COMMITTED FGameplayTag::RequestGameplayTag("AbilityEvent.Committed")
#define TAG_ABILITY_COMBO_WINDOW_ENABLED FGameplayTag::RequestGameplayTag("AbilityEvent.ComboWindowEnabled")
#define TAG_ABILITY_COMBO_ACTIVATED FGameplayTag::RequestGameplayTag("AbilityEvent.ComboActivated")
#define TAG_ABILITY_ATTACK_COMMITTED FGameplayTag::RequestGameplayTag("State.AttackCommitted")
#define TAG_ABILITY_ACTIVE FGameplayTag::RequestGameplayTag("State.Active")

#define TAG_STATE_AIMING FGameplayTag::RequestGameplayTag("State.Aiming")
#define TAG_STATE_STUNNED FGameplayTag::RequestGameplayTag("State.Stunned")
#define TAG_STATE_IMMOVABLE FGameplayTag::RequestGameplayTag("State.Immovable")
#define TAG_STATE_CANNOT_SPRINT FGameplayTag::RequestGameplayTag("State.CannotSprint")
#define TAG_STATE_ALOOF FGameplayTag::RequestGameplayTag("State.Aloof")
#define TAG_STATE_LOADING FGameplayTag::RequestGameplayTag("State.Loading")
#define TAG_STATE_ARMOR_PIERCING FGameplayTag::RequestGameplayTag("State.ArmorPiercing")
#define TAG_STATE_DEAD FGameplayTag::RequestGameplayTag("State.Dead")
#define TAG_STATE_FIRING FGameplayTag::RequestGameplayTag("State.Firing")
#define TAG_STATE_READY FGameplayTag::RequestGameplayTag("State.Ready")
#define TAG_STATE_RAGDOLL FGameplayTag::RequestGameplayTag("State.Ragdoll")
#define TAG_STATE_IN_COVER FGameplayTag::RequestGameplayTag("State.InCover")
#define TAG_STATE_CANNOT_MOUNT FGameplayTag::RequestGameplayTag("State.CannotMount")
#define TAG_STATE_IN_COMBAT FGameplayTag::RequestGameplayTag("State.InCombat")
#define TAG_STATE_ACTIVE FGameplayTag::RequestGameplayTag("State.Active")