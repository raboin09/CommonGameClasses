// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActorComponent/InteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "Types/CommonCoreTypes.h"
#include "CommonPlayerController.generated.h"

class UInputMappingContext;
class ACommonPlayerCharacter;
class UInteractionComponent;

UCLASS(Abstract, Blueprintable, CollapseCategories, AutoExpandCategories=("COMMON"), PrioritizeCategories = ("COMMON"))
class COMMONGAMECLASSES_API ACommonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACommonPlayerController();

	//~ Begin APlayerController Interface 
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	//~ End APlayerController Interface	
	
protected:
	//////////////////////////////////////////////
	/// CommonPlayerController
	//////////////////////////////////////////////

	virtual void SetupInputActions();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerController")
	void BPI_SetupInputActions();
	
	UFUNCTION(BlueprintCallable, Category="COMMON|PlayerController")
	void TryStartInteractionWithCurrentInteractable();
	
	UFUNCTION(BlueprintCallable, Category="COMMON|PlayerController")
	void StopInteraction();
	
	UFUNCTION(BlueprintCallable, Category="COMMON|PlayerController")
	void SetNewCameraType(ECameraType NewCameraType);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerController")
	void BPI_NewCameraTypeSelected(ECameraType NewCameraType);
	
	void NewActorHovered(UInteractionComponent* NewHoveredInteractable, bool bShouldOutline);
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|PlayerController")
	void BPI_HandleNewActorHovered(const UInteractionComponent* NewHoveredInteractable, bool bShouldOutline);
	
private:
	void MoveToNewDestination(const FVector& MoveLocation);	
	void NewActorTargeted(const AActor* NewHoveredActor);

	UFUNCTION()
	void HandleNewAbilityEquipped(const FNewAbilityEquippedPayload& NewAbilityEquippedPayload);
	
	void Internal_TryAssignInteractable();
	FHitResult Internal_ScanForTargetedActors() const;
	void Internal_CheckDistanceToInteractActor();
	void Internal_ClearCheckDistTimer();
	void Internal_GetShooterCameraTargetedActor(FHitResult& HitResult) const;

	bool IsInRangeOfInteractable(const UInteractionComponent* InteractionComponent) const;
	bool IsValidInteractionHitResult(const FHitResult& HitResult) const;
	bool IsValidInteractionComponent(const TWeakObjectPtr<UInteractionComponent> HitInteractionComponent) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON")
	TArray<FGameplayTag> DefaultGameplayTags;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact")
	float InteractWithAlliesDistance = 150.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact")
	float InteractWithEnemiesDistance = 150.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact")
	ECameraType DefaultCameraType = ECameraType::ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact")
	float InteractTraceRadius = 15.f;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Interact|Debug")
	bool bDrawDebug = false;

	UPROPERTY(BlueprintReadOnly, Category="COMMON|Interact|Debug")
	ECameraType CurrentCameraType = ECameraType::None;
	
private:
	FCameraTypeChanged CameraTypeChanged;
	
	UPROPERTY()
	TWeakObjectPtr<ACommonPlayerCharacter> PlayerCharacter;	
	UPROPERTY()
	TWeakObjectPtr<UInteractionComponent> CurrentHoveredInteractionComponent;

	// This is different for each ability (a melee weapon would only want to outline enemies that are 300 away,
	// whereas a shotgun would want to be something like 1000 and a sniper rifle might be 3000.
	// Interactable items should always have the same distance.
	float ValidAbilityOutlineDistance = 1000.f;
	
	// Interact move-to variables
	TWeakObjectPtr<UInteractionComponent> TargetedInteractionComponent;
	const float DISTANCE_CHECK_RATE = 0.1f;
	const float MAX_DISTANCE_CHECK_TIME = 7.f;	
	float CachedDistanceCheckTime = 0.f;
	FTimerHandle Timer_InteractDistanceCheck;

public:
	FORCEINLINE TWeakObjectPtr<AActor> GetCurrentHoveredInteractionActor() const { return CurrentHoveredInteractionComponent.IsValid() ? CurrentHoveredInteractionComponent->GetOwner() : nullptr; };
	FORCEINLINE TWeakObjectPtr<UInteractionComponent> GetCurrentHoveredInteractionComponent() const { return CurrentHoveredInteractionComponent.Get(); };
	FORCEINLINE TWeakObjectPtr<ACommonPlayerCharacter> GetCommonPlayerCharacter() const { return PlayerCharacter; };
	FORCEINLINE ECameraType GetCameraType() const { return CurrentCameraType; };
	FORCEINLINE FCameraTypeChanged& OnCameraTypeChanged() { return CameraTypeChanged; };
};
