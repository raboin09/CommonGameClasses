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
	virtual void InitTriggerMechanism() {}
	virtual void PressTrigger() PURE_VIRTUAL(ITriggerMechanism::PressTrigger,)
	virtual void ReleaseTrigger() PURE_VIRTUAL(ITriggerMechanism::ReleaseTrigger,)
	
	// Most don't need this, only for combo + montage abilities 
	virtual void ResetTrigger() {}
	
	FORCEINLINE FTriggerEvent& OnTriggerPressed() { return TriggerPressedEvent; }
	FORCEINLINE FTriggerEvent& OnTriggerReleased() { return TriggerReleasedEvent; }
	
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
	
	FTriggerEvent TriggerPressedEvent;
	FTriggerEvent TriggerReleasedEvent;
};
