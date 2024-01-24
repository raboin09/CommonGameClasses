// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActorComponent/InteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "CommonPlayerController.generated.h"

class IInteractable;
class ACommonPlayerCharacter;
class UQuestManagerComponent;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACommonPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;
	
	//////////////////////////////////////////////
	/// CommonPlayerController
	//////////////////////////////////////////////
	
	FORCEINLINE UInteractionComponent* GetCurrentHoveredInteractionComponent() const { return CurrentHoveredInteractionComponent.Get(); };
	FORCEINLINE ACommonPlayerCharacter* GetCommonPlayerCharacter() const { return PlayerCharacter.Get(); };
	FORCEINLINE UQuestManagerComponent* GetQuestManager() const { return QuestManager; }
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON|PlayerController")
	void K2_HandleNewActorHovered(const UInteractionComponent* NewHoveredInteractable, bool bShouldOutline);
	virtual void K2_HandleNewActorHovered_Implementation(const UInteractionComponent* NewHoveredInteractable, bool bShouldOutline);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON|PlayerController")
	void K2_TryStartInteraction();
	virtual void K2_TryStartInteraction_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON|PlayerController")
	void K2_StopInteraction();
	virtual void K2_StopInteraction_Implementation();

	void MoveToNewDestination(const FVector& MoveLocation);
	
private:
	void Internal_TryAssignInteractable();
	FHitResult Internal_ScanForActorUnderCursor() const;
	bool IsValidInteractableActorUnderCursor(const FHitResult& HitResult) const;
	void OnNewActorTargeted(const AActor* NewHoveredActor);
	void Internal_CheckDistanceToInteractActor();
	bool IsInRangeOfInteractable(const UInteractionComponent* InteractionComponent) const;
	void Internal_ClearCheckDistTimer();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;	
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float InteractWithAlliesDistance = 150.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Defaults")
	float InteractWithEnemiesDistance = 150.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UQuestManagerComponent* QuestManager;
	
private:
	UPROPERTY()
	TWeakObjectPtr<ACommonPlayerCharacter> PlayerCharacter;
	
	UPROPERTY()
	TWeakObjectPtr<UInteractionComponent> CurrentHoveredInteractionComponent;

	// Interact move-to variables
	UPROPERTY()
	TWeakObjectPtr<UInteractionComponent> TargetedInteractionComponent;
	const float DISTANCE_CHECK_RATE = 0.1f;
	const float MAX_DISTANCE_CHECK_TIME = 7.f;	
	float CachedDistanceCheckTime = 0.f;
	FTimerHandle Timer_InteractDistanceCheck;
};
