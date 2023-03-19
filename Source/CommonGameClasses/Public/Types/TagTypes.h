// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define TAG_ABILITY_ON_COOLDOWN FGameplayTag::RequestGameplayTag("AbilityEvent.OnCooldown")
#define TAG_ABILITY_ACTIVATED FGameplayTag::RequestGameplayTag("AbilityEvent.Activated")
#define TAG_ABILITY_REQUESTING_START FGameplayTag::RequestGameplayTag("AbilityEvent.RequestingStart")
#define TAG_ABILITY_COMMITTED FGameplayTag::RequestGameplayTag("AbilityEvent.Committed")
#define TAG_ABILITY_COMBO_WINDOW_ENABLED FGameplayTag::RequestGameplayTag("AbilityEvent.ComboWindowEnabled")
#define TAG_ABILITY_COMBO_ACTIVATED FGameplayTag::RequestGameplayTag("AbilityEvent.ComboActivated")

#define TAG_STATE_AIMING FGameplayTag::RequestGameplayTag("State.Aiming")