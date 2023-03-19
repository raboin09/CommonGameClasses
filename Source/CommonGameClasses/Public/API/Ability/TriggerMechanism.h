// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TriggerMechanism.generated.h"

UINTERFACE(MinimalAPI)
class UTriggerMechanism : public UInterface
{
	GENERATED_BODY()
};

class COMMONGAMECLASSES_API ITriggerMechanism
{
	GENERATED_BODY()
public:
	virtual void PressTrigger() PURE_VIRTUAL(ITriggerMechanism::PressTrigger,)
	virtual void ReleaseTrigger() PURE_VIRTUAL(ITriggerMechanism::ReleaseTrigger,)
	virtual bool DoesMontageControlActivation() const PURE_VIRTUAL(ITriggerMechanism::DoesMontageControlActivation, return false;)

	// Useful for burst triggers (once cooldown is done, ability hasn't necessarily ended)
	virtual bool ShouldRetriggerAbilityAfterCooldown() const PURE_VIRTUAL(ITriggerMechanism::ShouldRetriggerAbilityAfterCooldown, return false;)
	// Most don't need this, only for combo + montage abilities 
	virtual void ResetTrigger() {}
	
	DECLARE_EVENT_OneParam(ITriggerMechanism, FTriggerActivatedEvent, const FTriggerActivatedEventPayload&);
	FORCEINLINE FTriggerActivatedEvent& OnTriggerActivated() { return TriggerActivatedEvent; }
	DECLARE_EVENT_OneParam(ITriggerMechanism, FTriggerDeactivatedEvent, const FTriggerDeactivatedEventPayload&);
	FORCEINLINE FTriggerDeactivatedEvent& OnTriggerDeactivated() { return TriggerDeactivatedEvent; }

protected:
	FTriggerActivatedEvent TriggerActivatedEvent;
	FTriggerDeactivatedEvent TriggerDeactivatedEvent;
};
