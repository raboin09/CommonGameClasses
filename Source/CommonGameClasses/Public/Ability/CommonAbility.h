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

UCLASS(Abstract, Blueprintable)
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
	
	// Begin IAbility interface
	virtual void EquipAbility() override;
	virtual void UnEquipAbility() override;
	virtual bool TryStartAbility() override;
	virtual bool TryEndAbility() override;
	virtual void InitAbility(UMeshComponent* OwnerMeshComponent) override;
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

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Cooldown")
	bool bHasCooldown = false;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Cooldown", meta=(EditCondition="bHasCooldown == true", EditConditionHides))
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
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Trigger", meta=(MustImplement = "/Script/CommonGameClasses.TriggerMechanism", AllowedClasses="/Script/CommonGameClasses.ComplexTriggerBase", EditCondition="TriggerType == EAbilityTriggerType::Complex", EditConditionHides))
	TSubclassOf<UObject> ComplexTriggerClass;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation")
	TSubclassOf<UBaseActivation> ActivationMechanismClass;

	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Mesh")
	EMeshType MeshType = EMeshType::AbilityMesh;
	UPROPERTY(EditDefaultsOnly, Category="COMMON|Activation|Mesh")
	FName AttachmentSocket = "hand_r";

	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Resource", meta = (ClampMin = "0"))
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