// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CommonActor.h"
#include "API/Ability/Ability.h"
#include "Types/CommonAbilityTypes.h"
#include "Types/CommonEventDeclarations.h"
#include "CommonAbility.generated.h"

class UBehaviorTree;
class UCooldownMechanismImpl;
class IResourceContainer;
class USphereComponent;
class UBaseComplexTrigger;
class UAbilityComponent;
class UBaseActivation;
class UBaseTrigger;
class UBurstTrigger;
enum class EResourceContainerLocation : uint8;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM|Ability"))
class COMMONGAMECLASSES_API ACommonAbility : public ACommonActor, public IAbility
{
	GENERATED_BODY()
	
public:
	ACommonAbility();
	
	virtual void EquipAbility() override;
	virtual void UnEquipAbility() override;
	virtual bool TryStartAbility() override;
	virtual bool TryEndAbility() override;
	virtual void InitAbility(UMeshComponent* OwnerMeshComponent) override;
	virtual void DestroyAbility() override;
	virtual float GetAbilityOutlineRange() const override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void HandleEquipFinished();

	// Return how long to delay the execution of HandleEquipFinished (e.g. if playing an equip anim montage, return the duration of the montage)
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	float K2_HandleEquip();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleEquipFinished();
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Ability")
	void K2_HandleUnEquip();
	
	UFUNCTION(BlueprintCallable)
	float PlayAnimMontage(UAnimMontage* MontageToPlay);
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category="CUSTOM")
	TObjectPtr<UCooldownMechanismImpl> CooldownMechanism;
	// If the ability only requires simple trigger logic (only needs burst timer and/or num shots), create an instance of an obj instead of requiring a child BP.
	UPROPERTY(EditDefaultsOnly, Instanced, Category="CUSTOM", meta=(EditCondition = "ComplexTriggerClass == nullptr"))
	TObjectPtr<UBurstTrigger> SimpleTriggerInstance;
	// If the ability has more complex trigger logic (playing montages, listening for events, etc), a child BP obj is required.
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM", meta=(EditCondition = "SimpleTriggerInstance == nullptr"))
	TSubclassOf<UBaseComplexTrigger> ComplexTriggerClass;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	TSubclassOf<UBaseActivation> ActivationMechanismClass;

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Mesh")
	EMeshType MeshType = EMeshType::AbilityMesh;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Mesh")
	FName AttachmentSocket = "hand_r";

	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Resource", meta = (ClampMin = "0"))
	float ResourceCost = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Resource", meta = (EditCondition = "ResourceCost > 0"))
	EResourceContainerLocation ResourceContainerLocation;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Resource", meta = (MustImplement = "/Script/CommonGameClasses.ResourceContainer", EditCondition = "ResourceCost > 0 && (ResourceContainerLocation == EResourceContainerLocation::InstigatorComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerControllerComponent || ResourceContainerLocation == EResourceContainerLocation::AbilityComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerStateComponent)"))
	TSubclassOf<UActorComponent> ResourceContainerClass;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> AbilityStaticMesh;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> AbilitySkeletalMesh;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> AbilityRoot;dddddddddddd
	
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

	UPROPERTY()
	TWeakObjectPtr<UMeshComponent> MeshToUse;
	UPROPERTY()
	TWeakObjectPtr<UAbilityComponent> OwningAbilityComponent;
	
	UPROPERTY()
	TScriptInterface<IResourceContainer> ResourceContainer;
	UPROPERTY()
	TScriptInterface<ITriggerMechanism> TriggerMechanism;
	UPROPERTY()
	TScriptInterface<IActivationMechanism> ActivationMechanism;
	
	UPROPERTY()
	FTimerHandle Timer_OnEquipFinished;
}; 