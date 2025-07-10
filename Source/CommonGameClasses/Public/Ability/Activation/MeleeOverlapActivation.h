﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Activation/BaseActivation.h"
#include "API/Ability/ActivationMechanism.h"
#include "GameFramework/Actor.h"
#include "MeleeOverlapActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API UMeleeOverlapActivation : public UBaseActivation
{
	GENERATED_BODY()

public:
	//~ Begin UObject Interface
	virtual void PostInitProperties() override;
	//~ End UObject Interface
	
	virtual void InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent) override;	
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual void Deactivate() override;
	FORCEINLINE virtual float GetOutlineRange() const override { return MELEE_OUTLINE_DISTANCE; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BPI_HandleMontageActivation();
	UFUNCTION(BlueprintImplementableEvent)
	void BPI_HandleMontageDeactivation();
	UFUNCTION(BlueprintImplementableEvent)
	void BPI_HandleValidHit(FHitResult HitResult);
	UFUNCTION(BlueprintImplementableEvent)
	void BPI_HandleWeaponTraceStart();
	UFUNCTION(BlueprintImplementableEvent)
	void BPI_HandleWeaponTraceEnd();

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceForObjectTypes;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	float TraceRadius = 10.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	bool bCanHitMultipleEnemies = true;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	bool bFriendlyFire = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	FName Socket_TraceStart = "TraceStart";
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	FName Socket_TraceEnd = "TraceEnd";
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	float TraceTickRate = .01f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> AbilityEffects;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Debug")
	bool bDrawDebug = false;
	
private:
	UFUNCTION()
	void Internal_StartCollisionRaycastingTick();
	UFUNCTION()
	void Internal_StopCollisionRaycastingTick();
	UFUNCTION()
	void Internal_CheckForCollisionHit();
	void Internal_StartAttack();
	void Internal_StopAttack();

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> HitActors;
	UPROPERTY(Transient)
	bool bRecordedHit;
	
	FTimerHandle Timer_Raycasting;
};
