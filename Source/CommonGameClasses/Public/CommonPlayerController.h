﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerCharacter.h"
#include "GameplayTagContainer.h"
#include "Interactable.h"
#include "GameFramework/PlayerController.h"
#include "CommonPlayerController.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACommonPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	//////////////////////////////////////////////
	/// CommonPlayerController
	//////////////////////////////////////////////
	
	FORCEINLINE TScriptInterface<IInteractable> GetTargetedActor() const { return CurrentHoveredInteractable; };
	FORCEINLINE ACommonPlayerCharacter* GetCommonPlayerCharacter() const { return PlayerCharacter; };

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON|PlayerController")
	void K2_HandleNewActorHovered(const TScriptInterface<IInteractable>& NewHoveredInteractable, bool bShouldOutline);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON|PlayerController")
	void K2_InteractWithCurrentHoveredActor();

	void MoveToNewDestination(const FVector& MoveLocation);
	
private:
	void Internal_TryAssignInteractable();
	FHitResult Internal_ScanForActorUnderCursor() const;
	bool IsValidInteractableActorUnderCursor(const FHitResult& HitResult) const;
	void OnNewActorTargeted(AActor* NewHoveredActor);
	void Internal_CheckDistanceToInteractActor();
	bool IsInRangeOfInteractable(TScriptInterface<IInteractable> InInteractable, const AActor* InActor) const;
	void Internal_ClearCheckDistTimer();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float InteractWithAlliesDistance = 150.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float InteractWithEnemiesDistance = 150.f;

private:
	UPROPERTY()
	ACommonPlayerCharacter* PlayerCharacter;
	
	UPROPERTY()
	TScriptInterface<IInteractable> CurrentHoveredInteractable;
	UPROPERTY()
	AActor* CurrentHoveredActor;

	// Interact move-to variables
	UPROPERTY()
	TScriptInterface<IInteractable> TargetedInteractable;
	UPROPERTY()
	AActor* TargetedActor;
	const float DISTANCE_CHECK_RATE = 0.1f;
	const float MAX_DISTANCE_CHECK_TIME = 7.f;	
	float CachedDistanceCheckTime = 0.f;
	FTimerHandle Timer_InteractDistanceCheck;
};
