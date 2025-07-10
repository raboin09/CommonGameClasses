// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/Ability.h"
#include "Player/AbilityOutliner.h"
#include "Trigger/ComplexTriggerBase.h"
#include "Types/CommonAbilityTypes.h"
#include "Types/CommonEventDeclarations.h"
#include "CommonAbility.generated.h"

class UMontageTrigger;
class UBehaviorTree;
class UCooldownMechanismImpl;
class IResourceContainer;
class USphereComponent;
class UAbilityComponent;
class UBaseActivation;
class UAbilityTriggerBase;
enum class EResourceContainerLocation : uint8;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("COMMON"), PrioritizeCategories = ("COMMON"), HideCategories=("Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "Physics", "WorldPartition", "LevelInstance", "Cooking", "DataLayers", "Level Instance", "World Partition"))
class COMMONGAMECLASSES_API ACommonAbility : public ACommonActor, public IAbility, public IAbilityOutliner
{
	GENERATED_BODY()
	
public:
	ACommonAbility();

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	//~ End AActor Interface

	/**
	 * Equips the ability for this actor, triggering necessary gameplay events and operations.
	 *
	 * This method performs the following actions:
	 *   - Reveals the associated mesh if it was previously hidden using Internal_HideMesh.
	 *   - Adds an "Equipping" gameplay tag to the actor using UGameplayTagComponent.
	 *   - Calls an optional blueprint-implemented method, BPI_HandleEquip, to determine the duration of the equipping process.
	 *     - If the returned duration is greater than zero, a timer is set to call HandleEquipFinished after the specified duration.
	 *     - If the duration is zero or less, HandleEquipFinished is called immediately.
	 *
	 * The method ensures that any equip-related animations or logic are properly completed before the ability is fully equipped.
	 */
	virtual void EquipAbility() override;
	
	/**
	 * Unequips the ability for this actor, reverting visual and gameplay changes associated with it.
	 *
	 * This method performs the following actions:
	 *   - Attempts to end the ability by calling TryEndAbility, which may handle any cleanup logic for the ability.
	 *   - Hides the associated mesh of the ability by invoking Internal_HideMesh with the parameter set to true.
	 *   - Executes an optional blueprint-implemented method, BPI_HandleUnEquip, to allow for custom unequip behavior or logic implemented in derived blueprints.
	 *
	 * The method ensures the ability is properly deactivated and any visuals or mechanics tied to the ability are reset or hidden.
	 */
	virtual void UnEquipAbility() override;
	
	/**
	 * Attempts to start the ability for this actor, evaluating prerequisites and internal states.
	 *
	 * This method performs several checks and operations before starting the ability:
	 *   - Prevents the ability from starting if the actor has certain gameplay tags such as "RequestingStart" or "AutoStartAbility".
	 *   - If the actor is in the state of equipping, it automatically prepares the ability for activation.
	 *   - Handles the "ComboWindowEnabled" state by validating the trigger and activation mechanisms, updating relevant gameplay tags, and proceeding with combo activation if necessary.
	 *   - Ensures the ability does not start if the actor is on cooldown or already in an "Active" state.
	 *   - Evaluates the presence of trigger and activation mechanisms, auto-starting the ability when no trigger is present.
	 *   - Pauses resource regeneration if the ability has a resource cost and a valid resource container.
	 *   - Initiates the normal ability trigger sequence as the final step if all checks and prerequisites are fulfilled.
	 *
	 * @return true if the ability successfully starts or prepares for activation, false otherwise.
	 */
	virtual bool TryStartAbility() override;
	
	/**
	 * Attempts to end the currently active ability, ensuring all necessary cleanup and finishing actions are executed.
	 *
	 * This method performs the following operations:
	 *   - Cancels any ongoing ability tasks or timers that are related to this ability.
	 *   - Removes any gameplay tags associated with this ability from the actor using UGameplayTagComponent.
	 *   - Calls an optional blueprint-implemented method, BPI_HandleAbilityEnd, to handle any custom end-of-ability behavior.
	 *     - If BPI_HandleAbilityEnd defines a non-zero duration, a timer will be set to finish the ability after the specified time.
	 *     - If the duration is zero or less, the ability ends immediately without delay.
	 *   - Triggers any finalization logic required to transition the actor's state or gameplay systems to a post-ability state.
	 *
	 * @return True if the ability was successfully ended; otherwise, false if the ability could not be ended due to conditions such as being in progress or locked.
	 */
	virtual bool TryEndAbility() override;
	
	/**
	 * Initializes the ability for the actor, setting up required gameplay components and initial states.
	 *
	 * This method performs the following actions:
	 *   - Configures and binds key gameplay components or attributes necessary for the ability's functionality.
	 *   - Updates internal state to indicate the ability is ready for activation.
	 *   - Optionally invokes a blueprint-implemented method, BPI_HandleInit, to handle custom initialization logic.
	 *
	 * The method ensures that the ability is properly prepared and fully integrated into the actor's gameplay systems.
	 */
	virtual void InitAbility(UMeshComponent* OwnerMeshComponent) override;
	
	/**
	 * Destroys the current ability for this actor, ensuring proper cleanup and triggering associated events.
	 *
	 * This method performs the following actions:
	 *   - Removes any gameplay tags associated with the ability to ensure the actor’s state remains consistent.
	 *   - Optionally hides the ability’s visual components by calling Internal_HideMesh to manage cleanup visibility.
	 *   - Calls a blueprint-implemented method, BPI_HandleDestroy, which allows additional custom teardown logic to be executed.
	 *   - Ensures that any temporary effects or ongoing interactions tied to the ability are properly terminated.
	 *
	 * The method is designed to safely destroy the ability while maintaining the actor’s stability and avoiding potential state corruption.
	 */
	virtual void DestroyAbility() override;
	//~ End IAbility interface

	// Begin IAbilityOutliner interface
	virtual float GetAbilityOutlineRange() const override;
	//~ End IAbilityOutliner interface
	
protected:
	UFUNCTION()
	void HandleEquipFinished();

	// Return how long to delay the execution of HandleEquipFinished (e.g. if playing an equip anim montage, return the duration of the montage)
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	float BPI_HandleEquip();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_HandleEquipFinished();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void BPI_HandleUnEquip();
	
	UFUNCTION(BlueprintCallable, Category="COMMON|Ability")
	float PlayAnimMontage(UAnimMontage* MontageToPlay);	

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Aiming|TopDown", meta=(MustImplement = "/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> OwnerApplyEffectsOnAbilityStart;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Aiming|TopDown", meta=(MustImplement = "/Script/CommonGameClasses.Effect"))
	TArray<TSubclassOf<AActor>> OwnerApplyEffectsOnAbilityEnd;
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Aiming|TopDown")
	bool bEnableTwinStickAiming = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Aiming|TopDown")
	bool bDisableMovementRotationOrientationOnAbilityStart = false;
	
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Cooldown", meta=(InlineEditConditionToggle))
	bool bHasCooldown = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Cooldown", meta=(EditCondition="bHasCooldown == true", ClampMin=0))
	float CooldownDuration = 1.f;
	
	// If the ability only requires simple trigger logic (only needs burst timer and/or num shots), create an instance of an obj instead of requiring a child BP.
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger")
	EAbilityTriggerType TriggerType = EAbilityTriggerType::SimpleBurst;

	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", DisplayName="NumberOfActivations", meta=(ClampMin = 0.f, EditCondition = "TriggerType == EAbilityTriggerType::SimpleBurst", EditConditionHides))
	int32 BurstTrigger_NumberOfActivations = 1;
	// The delay (if any) before activating again (e.g. a shotgun would be 0 as all the pellets fire instantaneously, a 3-round burst rifle would be
	// something like .1)
	UPROPERTY(EditAnywhere, Category="COMMON|Trigger", DisplayName="TimeBetweenBurstShots", meta=(EditCondition = "BurstTrigger_NumberOfActivations > 1 && TriggerType == EAbilityTriggerType::SimpleBurst", EditConditionHides, ClampMin = 0.f))
	float BurstTrigger_TimeBetweenBurstShots = .1f;

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", DisplayName="MontageToPlay", meta=(EditCondition = "TriggerType == EAbilityTriggerType::SimpleMontage", EditConditionHides))
	TObjectPtr<UAnimMontage> MontageTrigger_MontageToPlay;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", DisplayName="HasCombos", meta=(EditCondition = "TriggerType == EAbilityTriggerType::SimpleMontage", EditConditionHides))
	bool MontageTrigger_HasCombos = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", DisplayName="MaxComboSections", meta=(EditCondition = "MontageTrigger_HasCombos == true && TriggerType == EAbilityTriggerType::SimpleMontage", EditConditionHides))
	int32 MontageTrigger_MaxComboSections = 3;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", DisplayName="RandomizeMontageSection", meta=(EditCondition = "MontageTrigger_HasCombos == true  && TriggerType == EAbilityTriggerType::SimpleMontage", EditConditionHides))
	bool MontageTrigger_RandomizeMontageSection = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", DisplayName="ComboPrefix", meta=(EditCondition = "MontageTrigger_HasCombos == true  && TriggerType == EAbilityTriggerType::SimpleMontage", EditConditionHides))
	FString MontageTrigger_ComboPrefix = "Combo";
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", DisplayName="ShouldPlayerLockOnToNearestTarget", meta=(EditCondition = "TriggerType == EAbilityTriggerType::SimpleMontage", EditConditionHides))
	bool MontageTrigger_ShouldPlayerLockOnToNearestTarget = false;
	
	// If the ability has more complex trigger logic a child BP obj is required.
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", meta=(MustImplement = "/Script/CommonGameClasses.TriggerMechanism", EditCondition="TriggerType == EAbilityTriggerType::Complex", EditConditionHides))
	TSubclassOf<UObject> ComplexTriggerClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation", meta=(MustImplement = "/Script/CommonGameClasses.ActivationMechanism"))
	TSubclassOf<UObject> ActivationMechanismClass;

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Mesh")
	EMeshType MeshType = EMeshType::AbilityMesh;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Mesh", meta=(EditCondition="MeshType != EMeshType::None"))
	FName AttachmentSocket = "hand_r";

	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Resource", meta=(InlineEditConditionToggle))
	bool bHasResourceCost = false;
	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Resource", meta = (ClampMin = "0", EditCondition = "bHasResourceCost"))
	float ResourceCost = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Resource", meta = (EditCondition = "ResourceCost > 0"))
	EResourceContainerLocation ResourceContainerLocation;
	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Resource", meta = (MustImplement = "/Script/CommonGameClasses.ResourceContainer", EditCondition = "ResourceCost > 0 && (ResourceContainerLocation == EResourceContainerLocation::InstigatorComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerControllerComponent || ResourceContainerLocation == EResourceContainerLocation::AbilityComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerStateComponent)"))
	TSubclassOf<UActorComponent> ResourceContainerClass;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> AbilityStaticMesh;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> AbilitySkeletalMesh;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> AbilityRoot;
	
private:
	void InitWeaponMesh(UMeshComponent* InMeshComp) const;
	void Internal_SetMeshToUse();
	void Internal_HideMesh(bool bShouldHide) const;
	
	void SetTriggerMechanism();
	void SetResourceContainerObject();
	void SetActivationMechanism();

	void Internal_SetResourceContainerToObject(TWeakObjectPtr<UObject> ContainerObject);
	void Internal_SetResourceContainerToComponent(TWeakObjectPtr<const AActor> PotentialActor);
	bool Internal_StartNormalAbility();
	TObjectPtr<UObject> Internal_CreateNewMechanism(const TSubclassOf<UObject> InMechanismClass, const UClass* InterfaceClass);
	void Internal_BindMechanismEventsToAbility();
	void Internal_TryTogglePauseResourceRegeneration(bool bShouldPause) const;
	void Internal_TryUpdateAimingState(bool bStartingAbility) const;
	void Internal_TryUpdateMovementOrientationState(bool bStartingAbility) const;

	UFUNCTION()
	void HandleAbilityActivationEvent(const FAbilityActivationEventPayload& AbilityActivationEventPayload);
	UFUNCTION()
	void HandleAbilityDeactivationEvent(const FAbilityDeactivationEventPayload& AbilityDeactivationEventPayload);
	UFUNCTION()
	void HandleTriggerPressedEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload) const;
	UFUNCTION()
	void HandleTriggerReleasedEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload);
	UFUNCTION()
	void HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent);
	UFUNCTION()
	void HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent);

private:
	UPROPERTY()
	TWeakObjectPtr<UMeshComponent> MeshToUse;
	UPROPERTY()
	TWeakObjectPtr<UAbilityComponent> OwningAbilityComponent;

	UPROPERTY()
	TScriptInterface<ICooldownMechanism> CooldownMechanism;
	UPROPERTY()
	TScriptInterface<IResourceContainer> ResourceContainer;
	UPROPERTY()
	TScriptInterface<ITriggerMechanism> TriggerMechanism;
	UPROPERTY()
	TScriptInterface<IActivationMechanism> ActivationMechanism;
	
	UPROPERTY()
	FTimerHandle Timer_OnEquipFinished;
}; 