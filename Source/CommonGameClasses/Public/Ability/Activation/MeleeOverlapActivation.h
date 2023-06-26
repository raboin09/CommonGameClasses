// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Activation/ActivationBase.h"
#include "API/Ability/ActivationMechanism.h"
#include "GameFramework/Actor.h"
#include "MeleeOverlapActivation.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API AMeleeOverlapActivation : public AActivationBase
{
	GENERATED_BODY()

public:
	AMeleeOverlapActivation();
	virtual void BeginPlay() override;
	
	virtual void Activate(const FTriggerEventPayload& TriggerEventPayload) override;
	virtual void Deactivate() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void K2_WeaponActivated();
	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayHitEffects(FHitResult HitResult);
	UFUNCTION(BlueprintImplementableEvent)
	void K2_StartWeaponTrace();
	UFUNCTION(BlueprintImplementableEvent)
	void K2_StopWeaponTrace();
	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Fire")
	float TraceRadius = 35.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Fire")
	bool bCanHitMultipleEnemies = true;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Fire")
	bool bFriendlyFire = false;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Animation")
	FString SocketPrefix = "R_";
	UPROPERTY(BlueprintReadOnly)
	FName CachedComboSection;
	
private:
	UFUNCTION()
	void Internal_StartCollisionRaycastingTick();
	UFUNCTION()
	void Internal_StopCollisionRaycastingTick();
	UFUNCTION()
	void Internal_CheckForCollisionHit();
	void Internal_SetCurrentSocketLocations();
	void Internal_StartAttack();
	void Internal_StopAttack();
	
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability", meta=(MustImplement="/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> AbilityEffects;

	TMap<FString, FVector> Sockets;
	FTimerHandle Timer_Raycasting;
	
	bool bRecordedHit;
public:
	FORCEINLINE virtual void SetAbilityMesh(UMeshComponent* InMeshComponent) override { MeshComponentRef = InMeshComponent; }
};
