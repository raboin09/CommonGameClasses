// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonEventDeclarations.h"
#include "UObject/Interface.h"
#include "CooldownMechanism.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCooldownMechanism : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API ICooldownMechanism
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void StartCooldownTimer() = 0;
	UFUNCTION()
	virtual void StopCooldownTimer() = 0;
	UFUNCTION()
	virtual void PauseCooldownTimer() = 0;
	
	FORCEINLINE FCooldownTickedEvent& OnCooldownTicked() { return CooldownTickedEvent; }
	FORCEINLINE FCooldownStartedEvent& OnCooldownTimerStarted() { return CooldownStartedEvent; }
	FORCEINLINE FCooldownEndedEvent& OnCooldownTimerEnded() { return CooldownEndedEvent; }

	FORCEINLINE virtual void SetInstigator(TWeakObjectPtr<APawn> InPawn) { PawnInstigator = InPawn; }
	FORCEINLINE virtual void SetOwner(TWeakObjectPtr<AActor> InActor) { AbilityOwner = InActor; }
	
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual APawn* GetInstigator() const { return PawnInstigator.Get(); }
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	virtual AActor* GetOwner() const { return AbilityOwner.Get(); }
	
protected:
	FORCEINLINE virtual UWorld* GetOwnerWorld() const { return GetInstigator() ? GetInstigator()->GetWorld() : nullptr; }
	TWeakObjectPtr<AActor> AbilityOwner = nullptr;
	TWeakObjectPtr<APawn> PawnInstigator = nullptr;
	
	FCooldownTickedEvent CooldownTickedEvent;
	FCooldownStartedEvent CooldownStartedEvent;
	FCooldownEndedEvent CooldownEndedEvent;
};

