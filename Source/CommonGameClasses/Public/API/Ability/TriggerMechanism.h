// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "UObject/Interface.h"
#include "TriggerMechanism.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UTriggerMechanism : public UInterface
{
	GENERATED_BODY()
};

class COMMONGAMECLASSES_API ITriggerMechanism
{
	GENERATED_BODY()
public:
	virtual void InitTrigger() {}
	virtual void PressTrigger() PURE_VIRTUAL(ITriggerMechanism::PressTrigger,)
	virtual void ReleaseTrigger() PURE_VIRTUAL(ITriggerMechanism::ReleaseTrigger,)
	
	// Useful for burst triggers (once cooldown is done, ability hasn't necessarily ended)
	virtual bool ShouldRetriggerAbilityAfterCooldown() const PURE_VIRTUAL(ITriggerMechanism::ShouldRetriggerAbilityAfterCooldown, return false;)
	// Most don't need this, only for combo + montage abilities 
	virtual void ResetTrigger() {}
	
	FORCEINLINE FTriggerEvent& OnTriggerPressed() { return TriggerPressedEvent; }
	FORCEINLINE FTriggerEvent& OnTriggerReleased() { return TriggerReleasedEvent; }
	
	FORCEINLINE virtual void SetInstigator(APawn* InPawn) { PawnInstigator = InPawn; }
	FORCEINLINE virtual APawn* GetInstigator() const { return PawnInstigator; }
	FORCEINLINE virtual void SetOwner(AActor* InActor) { AbilityOwner = InActor; }
	FORCEINLINE virtual AActor* GetOwner() const { return AbilityOwner; }
	
protected:
	AActor* AbilityOwner = nullptr;
	APawn* PawnInstigator = nullptr;
	
	FTriggerEvent TriggerPressedEvent;
	FTriggerEvent TriggerReleasedEvent;
};
