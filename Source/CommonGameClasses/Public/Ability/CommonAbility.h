// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CooldownMechanismImpl.h"
#include "Activation/BaseActivation.h"
#include "ActorComponent/AbilityComponent.h"
#include "ActorComponent/ResourceComponent.h"
#include "Actors/CommonActor.h"
#include "API/Ability/Ability.h"
#include "Trigger/BaseTrigger.h"
#include "Types/CommonAbilityTypes.h"
#include "Types/CommonEventDeclarations.h"
#include "Types/CommonTagTypes.h"
#include "CommonAbility.generated.h"

class IResourceContainer;
class USphereComponent;
enum class EResourceContainerLocation : uint8;

UCLASS(Abstract, Blueprintable, AutoExpandCategories=("CUSTOM|Ability"))
class COMMONGAMECLASSES_API ACommonAbility : public ACommonActor, public IAbility
{
	GENERATED_BODY()
	
public:
	ACommonAbility();
	virtual bool TryStartAbility() override;
	virtual bool TryEndAbility() override;
	virtual void InitAbility(UMeshComponent* OwnerMeshComponent) override;
	FORCEINLINE virtual UMeshComponent* GetAbilityMesh() const override { return MeshToUse; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability|Mesh")
	FGameplayTag AbilityOverlay = FGameplayTag(CommonGameAnimation::Unarmed);
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability|Mesh")
	EMeshType MeshType = EMeshType::AbilityMesh;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability|Mesh")
	FName AttachmentSocket = "hand_r";
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Ability|Resource", meta = (ClampMin = "0"))
	float ResourceCost = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Ability|Resource", meta = (EditCondition = "ResourceCost > 0"))
	EResourceContainerLocation ResourceContainerLocation;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Ability|Resource", meta = (MustImplement = "/Script/CommonGameClasses.ResourceContainer", EditCondition = "(ResourceContainerLocation == EResourceContainerLocation::InstigatorComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerControllerComponent || ResourceContainerLocation == EResourceContainerLocation::AbilityComponent || ResourceContainerLocation == EResourceContainerLocation::PlayerStateComponent)"))
	TSubclassOf<UActorComponent> ResourceContainerClass;
	UPROPERTY(EditDefaultsOnly, Instanced, Category="CUSTOM|Ability")
	TObjectPtr<UCooldownMechanismImpl> CooldownMechanism;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability")
	TSubclassOf<UBaseTrigger> TriggerMechanismClass;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM|Ability")
	TSubclassOf<UBaseActivation> ActivationMechanismClass;
	
	UPROPERTY(VisibleDefaultsOnly, Category="CUSTOM|Ability")
	USphereComponent* AbilityRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Ability")
	UStaticMeshComponent* AbilityStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Ability")
	USkeletalMeshComponent* AbilitySkeletalMesh;

private:
	void Internal_SetMeshToUse();
	void SetTriggerMechanism();
	void SetResourceContainerObject();
	void SetActivationMechanism();

	void Internal_SetResourceContainerToObject(UObject* ContainerObject);
	void Internal_SetResourceContainerToComponent(const AActor* PotentialActor);
	bool Internal_StartNormalAbility();
	UObject* Internal_CreateNewMechanism(const TSubclassOf<UObject> InMechanismClass, const UClass* InterfaceClass);
	void Internal_BindMechanismEventsToAbility();

	void InitWeaponMesh(UMeshComponent* InMeshComp) const;

	UFUNCTION()
	void HandleAbilityActivationEvent(const FAbilityActivationEventPayload& AbilityActivationEventPayload) const;
	UFUNCTION()
	void HandleAbilityDeactivationEvent(const FAbilityDeactivationEventPayload& AbilityDeactivationEventPayload) const;
	UFUNCTION()
	void HandleTriggerPressedEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload) const;
	UFUNCTION()
	void HandleTriggerReleasedEvent(const FTriggerEventPayload& AbilityTriggeredEventPayload);
	UFUNCTION()
	void HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent);
	UFUNCTION()
	void HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent);

	UPROPERTY()
	UMeshComponent* MeshToUse;
	UPROPERTY()
	TScriptInterface<IResourceContainer> ResourceContainer;
	UPROPERTY()
	TObjectPtr<UBaseTrigger> TriggerMechanism;
	UPROPERTY()
	TObjectPtr<UBaseActivation> ActivationMechanism;
	UPROPERTY()
	UAbilityComponent* OwningAbilityComponent;
}; 