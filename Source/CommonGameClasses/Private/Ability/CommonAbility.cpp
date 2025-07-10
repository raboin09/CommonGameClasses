#include "Ability/CommonAbility.h"

#include "Ability/CooldownMechanismImpl.h"
#include "Ability/Activation/BaseActivation.h"
#include "Ability/Trigger/BurstTrigger.h"
#include "ActorComponent/AbilityComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "API/Ability/ActivationMechanism.h"
#include "API/Ability/TriggerMechanism.h"
#include "Components/SphereComponent.h"
#include "Types/CommonTagTypes.h"
#include "API/Ability/ResourceContainer.h"
#include "Ability/Trigger/MontageTrigger.h"
#include "ActorComponent/TopDownAimingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Types/CommonAbilityTypes.h"
#include "Types/CommonCharacterAnimTypes.h"

ACommonAbility::ACommonAbility()
{
	AbilityRoot = CreateDefaultSubobject<USphereComponent>(TEXT("SphereRoot"));
	AbilityRoot->SetCollisionResponseToAllChannels(ECR_Ignore);
	AbilityRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(AbilityRoot);

	AbilitySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AbilitySkeletalMesh"));
	InitWeaponMesh(AbilitySkeletalMesh);

	AbilityStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AbilityStaticMesh"));
	InitWeaponMesh(AbilityStaticMesh);
}

void ACommonAbility::BeginPlay()
{
	Super::BeginPlay();

	if(bHasCooldown)
	{
		UCooldownMechanismImpl* TempCooldownObj = NewObject<UCooldownMechanismImpl>(this);
		TempCooldownObj->CooldownDuration = CooldownDuration;
		CooldownMechanism = TempCooldownObj;
		CooldownMechanism->SetInstigator(GetInstigator());
		CooldownMechanism->SetOwner(this);
	}
	
	SetActivationMechanism();
	SetResourceContainerObject();
	SetTriggerMechanism();
	Internal_BindMechanismEventsToAbility();
	if (!GetInstigator())
	{
		return;
	}
	OwningAbilityComponent = GetInstigator()->FindComponentByClass<UAbilityComponent>();
}

void ACommonAbility::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Internal_SetMeshToUse();
}

void ACommonAbility::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void ACommonAbility::InitWeaponMesh(UMeshComponent* InMeshComp) const
{
	if (!InMeshComp)
	{
		return;
	}

	InMeshComp->bReceivesDecals = false;
	InMeshComp->CastShadow = true;
	InMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	InMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	InMeshComp->SetupAttachment(AbilityRoot);
}

void ACommonAbility::EquipAbility()
{
	Internal_HideMesh(false);
	UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::Equipping);
	const float Duration = BPI_HandleEquip();
	if (Duration > 0.0f)
	{
		GetWorldTimerManager().SetTimer(Timer_OnEquipFinished, this, &ThisClass::HandleEquipFinished, Duration, false);
	}
	else
	{
		HandleEquipFinished();
	}
}

void ACommonAbility::HandleEquipFinished()
{
	UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::Equipping);
	if(UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameAbilityEvent::AutoStartAbility))
	{
		UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::AutoStartAbility);
		TryStartAbility();
	}
	BPI_HandleEquipFinished();
	if(bEnableTwinStickAiming)
	{
		if(UTopDownAimingComponent* TopDownAimingComponent = GetInstigator()->FindComponentByClass<UTopDownAimingComponent>())
		{
			TopDownAimingComponent->ToggleShouldRotateCharacterToAiming(true);
		}
	}
}

float ACommonAbility::PlayAnimMontage(UAnimMontage* MontageToPlay)
{
	UCharacterAnimationComponent* CharacterAnimationComponent = GetInstigator()->FindComponentByClass<UCharacterAnimationComponent>();
	if (!CharacterAnimationComponent || !MontageToPlay)
	{
		return 0.f;
	}

	BPI_HandleEquip();
	FAnimMontagePlayData PlayData = FAnimMontagePlayData();
	PlayData.MontageToPlay = MontageToPlay;
	return CharacterAnimationComponent->ForcePlayAnimMontage(PlayData);
}

void ACommonAbility::UnEquipAbility()
{
	TryEndAbility();
	Internal_HideMesh(true);
	BPI_HandleUnEquip();
	if(bEnableTwinStickAiming)
	{
		if(UTopDownAimingComponent* TopDownAimingComponent = GetInstigator()->FindComponentByClass<UTopDownAimingComponent>())
		{
			TopDownAimingComponent->ToggleShouldRotateCharacterToAiming(false);
		}
	}
}

bool ACommonAbility::TryStartAbility()
{
	if (UGameplayTagComponent::ActorHasAnyGameplayTags(this,{CommonGameAbilityEvent::RequestingStart, CommonGameAbilityEvent::AutoStartAbility}))
	{
		return false;
	}

	if(UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameAbilityEvent::Equipping))
	{
		UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::AutoStartAbility);
		return true;
	}

	if (UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameAbilityEvent::ComboWindowEnabled))
	{
		if (TriggerMechanism && ActivationMechanism)
		{
			UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::ComboActivated);
			UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::ComboWindowEnabled);
			if (UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameAbilityEvent::Active))
			{
				UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::AutoStartAbility);
				return true;
			}
		}
		return false;
	}
	if (UGameplayTagComponent::ActorHasAnyGameplayTags(this, {CommonGameAbilityEvent::OnCooldown, CommonGameAbilityEvent::Active}))
	{
		return false;
	}


	if (!TriggerMechanism)
	{
		// If there's no trigger or activation, is this really an ability?
		if (!ActivationMechanism)
		{
			return false;
		}

		// Activate instantly, no trigger exists
		UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::RequestingStart);
		ActivationMechanism->Activate(FTriggerEventPayload());
		return true;
	}
	
	if(ResourceContainer && bHasResourceCost)
	{
		ResourceContainer->TryTogglePauseResourceRegen(true);
	}
	// It's an ability with a Trigger (Activation and Cooldown mechanism may be present), so proceed to press the trigger
	return Internal_StartNormalAbility();
}

bool ACommonAbility::TryEndAbility()
{
	if (!TriggerMechanism)
	{
		return true;
	}
	TriggerMechanism->ReleaseTrigger();
	if(ResourceContainer && bHasResourceCost)
	{
		ResourceContainer->TryTogglePauseResourceRegen(false);
	}
	return true;
}

void ACommonAbility::InitAbility(UMeshComponent* OwnerMeshComponent)
{
	if (!OwnerMeshComponent || !MeshToUse.IsValid())
	{
		return;
	}
	
	if(MeshToUse != OwnerMeshComponent)
	{
		MeshToUse->AttachToComponent(OwnerMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, AttachmentSocket);	
	}
	Internal_HideMesh(true);
}

void ACommonAbility::DestroyAbility()
{
	Destroy();
}

float ACommonAbility::GetAbilityOutlineRange() const
{
	if(ActivationMechanism)
	{
		return ActivationMechanism->GetOutlineRange();
	}
	return DEFAULT_OUTLINE_DISTANCE;
}

void ACommonAbility::Internal_SetMeshToUse()
{
	if(MeshType == EMeshType::None)
	{
		return;
	}

	if(MeshType == EMeshType::AbilityMesh)
	{
		if(AbilitySkeletalMesh && AbilitySkeletalMesh->GetSkeletalMeshAsset())
		{
			MeshToUse = AbilitySkeletalMesh;
		}
	
		if(AbilityStaticMesh && AbilityStaticMesh->GetStaticMesh())
		{
			MeshToUse = AbilityStaticMesh;
		}
	}
	else if (MeshType == EMeshType::InstigatorMesh)
	{
		APawn* CurrInstigator = GetInstigator();
		if(!CurrInstigator)
		{
			return;
		}

		if(const ACharacter* CharOwner = Cast<ACharacter>(CurrInstigator))
		{
			MeshToUse =  CharOwner->GetMesh();
		}
		else if(UMeshComponent* MeshComp = CurrInstigator->FindComponentByClass<UMeshComponent>())
		{
			MeshToUse =  MeshComp;
		}
	}
}

void ACommonAbility::Internal_HideMesh(bool bShouldHide) const
{
	if (MeshToUse.IsValid() && MeshType == EMeshType::AbilityMesh)
	{
		MeshToUse->SetHiddenInGame(bShouldHide);
	}
}

void ACommonAbility::SetTriggerMechanism()
{
	if (TriggerType == EAbilityTriggerType::SimpleBurst)
	{
		UBurstTrigger* BurstTriggerInstance = NewObject<UBurstTrigger>(this);
		BurstTriggerInstance->NumberOfActivations = BurstTrigger_NumberOfActivations;
		BurstTriggerInstance->TimeBetweenBurstShots = BurstTrigger_TimeBetweenBurstShots;
		TriggerMechanism = BurstTriggerInstance;
	} else if(TriggerType == EAbilityTriggerType::SimpleMontage)
	{
		UMontageTrigger* MontageTriggerInstance = NewObject<UMontageTrigger>(this);
		MontageTriggerInstance->MontageToPlay = MontageTrigger_MontageToPlay;
		MontageTriggerInstance->bHasCombos = MontageTrigger_HasCombos;
		MontageTriggerInstance->bRandomizeMontageSection = MontageTrigger_RandomizeMontageSection;
		MontageTriggerInstance->bShouldPlayerLockOnToNearestTarget = MontageTrigger_ShouldPlayerLockOnToNearestTarget;
		MontageTriggerInstance->ComboPrefix = MontageTrigger_ComboPrefix;
		MontageTriggerInstance->MaxComboSections = MontageTrigger_MaxComboSections;
		TriggerMechanism = MontageTriggerInstance;
	}
	else
	{
		if(!ComplexTriggerClass)
		{
			return;
		}
		const TObjectPtr<UObject> TempObj = Internal_CreateNewMechanism(ComplexTriggerClass, UTriggerMechanism::StaticClass());
		TriggerMechanism.SetObject(TempObj);
		TriggerMechanism.SetInterface(Cast<ITriggerMechanism>(TempObj));
	}
	TriggerMechanism->SetInstigator(GetInstigator());
	TriggerMechanism->SetOwner(this);
	TriggerMechanism->InitTriggerMechanism();
}

void ACommonAbility::SetResourceContainerObject()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in this ability or a component contained in this ability
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if (ResourceContainerLocation == EResourceContainerLocation::Ability)
	{
		Internal_SetResourceContainerToObject(this);
		return;
	}
	if (ResourceContainerLocation == EResourceContainerLocation::AbilityComponent)
	{
		Internal_SetResourceContainerToComponent(this);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in Instigator or a component contained in Instigator
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	APawn* CurrInstigator = GetInstigator();
	if (!CurrInstigator)
	{
		return;
	}

	if (ResourceContainerLocation == EResourceContainerLocation::Instigator)
	{
		Internal_SetResourceContainerToObject(CurrInstigator);
		return;
	}
	if (ResourceContainerLocation == EResourceContainerLocation::InstigatorComponent)
	{
		Internal_SetResourceContainerToComponent(CurrInstigator);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in Instigator's Controller or a component contained in Instigator's Controller
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	AController* Controller = CurrInstigator->Controller;
	if (ResourceContainerLocation == EResourceContainerLocation::PlayerController && Controller)
	{
		Internal_SetResourceContainerToObject(Controller);
		return;
	}
	if (ResourceContainerLocation == EResourceContainerLocation::PlayerControllerComponent && Controller)
	{
		Internal_SetResourceContainerToComponent(Controller);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in Instigator's PlayerState or a component contained in Instigator's PlayerState
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	APlayerState* PlayerState = CurrInstigator->GetPlayerState();
	if (ResourceContainerLocation == EResourceContainerLocation::PlayerState && PlayerState)
	{
		Internal_SetResourceContainerToObject(PlayerState);
	}
	if (ResourceContainerLocation == EResourceContainerLocation::PlayerStateComponent && PlayerState)
	{
		Internal_SetResourceContainerToComponent(PlayerState);
	}
}

void ACommonAbility::Internal_SetResourceContainerToComponent(TWeakObjectPtr<const AActor> PotentialActor)
{
	if (!PotentialActor.IsValid())
		return;
	Internal_SetResourceContainerToObject(PotentialActor->GetComponentByClass(ResourceContainerClass.Get()));
}

void ACommonAbility::Internal_SetResourceContainerToObject(TWeakObjectPtr<UObject> ContainerObject)
{
	ResourceContainer.SetInterface(Cast<IResourceContainer>(ContainerObject));
	ResourceContainer.SetObject(ContainerObject.Get());
}

void ACommonAbility::SetActivationMechanism()
{
	UObject* TempObj = Internal_CreateNewMechanism(ActivationMechanismClass.Get(), UActivationMechanism::StaticClass());
	if (!TempObj || !TempObj->IsA(UBaseActivation::StaticClass()))
	{
		return;
	}
	ActivationMechanism = Cast<UBaseActivation>(TempObj);
	ActivationMechanism->SetInstigator(GetInstigator());
	ActivationMechanism->SetOwner(this);
	ActivationMechanism->InitActivationMechanism(MeshToUse.Get());
}

bool ACommonAbility::Internal_StartNormalAbility()
{
	// If no costs are required or has required resource cost, fire ability off
	if (!ResourceContainer || !bHasResourceCost || ResourceContainer->TryConsumeResourceAmount(ResourceCost))
	{
		UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::RequestingStart);
		UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::Active);
		TriggerMechanism->PressTrigger();
		return true;
	}
	return false;
}

TObjectPtr<UObject> ACommonAbility::Internal_CreateNewMechanism(const TSubclassOf<UObject> InMechanismClass, const UClass* InterfaceClass)
{
	if (!InMechanismClass || !InterfaceClass)
	{
		return nullptr;
	}
	
	if (!InMechanismClass->ImplementsInterface(InterfaceClass))
	{
		return nullptr;
	}
	UObject* SpawnedObj = NewObject<UObject>(this, InMechanismClass);
	return SpawnedObj;
}

void ACommonAbility::Internal_BindMechanismEventsToAbility()
{
	if (bHasCooldown && CooldownMechanism)
	{
		CooldownMechanism->OnCooldownTimerStarted().AddUObject(this, &ThisClass::HandleCooldownStarted);
		CooldownMechanism->OnCooldownTimerEnded().AddUObject(this, &ThisClass::HandleCooldownEnded);
	}

	if (TriggerMechanism)
	{
		TriggerMechanism->OnTriggerPressed().AddUObject(this, &ThisClass::HandleTriggerPressedEvent);
		TriggerMechanism->OnTriggerReleased().AddUObject(this, &ThisClass::HandleTriggerReleasedEvent);
	}

	if (ActivationMechanism)
	{
		ActivationMechanism->OnActivation().AddUObject(this, &ThisClass::HandleAbilityActivationEvent);
		ActivationMechanism->OnDeactivation().AddUObject(this, &ThisClass::HandleAbilityDeactivationEvent);
	}
}

void ACommonAbility::HandleAbilityDeactivationEvent( const FAbilityDeactivationEventPayload& AbilityDeactivationEventPayload)
{
	UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::Activated);
	UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::Active);
	if (UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameAbilityEvent::AutoStartAbility))
	{
		UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::AutoStartAbility);
		TryStartAbility();
	}
}

void ACommonAbility::HandleAbilityActivationEvent(const FAbilityActivationEventPayload& AbilityActivationEventPayload)
{
	UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::Activated);
	// Some activation events don't start cooldowns until an external event happens (e.g. montage notifies)
	if (AbilityActivationEventPayload.bShouldStartCooldown && CooldownMechanism && bHasCooldown)
	{
		CooldownMechanism->StartCooldownTimer();
	}
}

void ACommonAbility::HandleTriggerPressedEvent(const FTriggerEventPayload& TriggeredEventPayload) const
{
	// If there's a Trigger with no Activation (e.g. Jump just plays a montage) then start the cooldown immediately
	if (!ActivationMechanism)
	{
		if (CooldownMechanism && bHasCooldown)
		{
			CooldownMechanism->StartCooldownTimer();
		}
		return;
	}

	// Does the Activation happen immediately (e.g. firing a weapon) or should it wait (e.g. swinging a sword montage having an Activation notify)
	if (TriggeredEventPayload.bStartActivationImmediately)
	{
		// Pass some info from the Trigger to Activation (needed for things like charge-up weapons, throwing grenades with a predicted location, etc)
		ActivationMechanism->Activate(TriggeredEventPayload);
	}
	else
	{
		if (!OwningAbilityComponent.IsValid() || !TriggeredEventPayload.bMontageDrivesActivation)
		{
			return;
		}

		// Some activations don't start until an external event happens (e.g. montage notifies), so we store it
		// in the parent AbilityComponent
		FAwaitingActivationDetails AwaitingActivationDetails;
		AwaitingActivationDetails.MechanismAwaitingActivation = ActivationMechanism;
		AwaitingActivationDetails.TriggerActivationPayload = TriggeredEventPayload;
		OwningAbilityComponent->SetMechanismAwaitingActivation(AwaitingActivationDetails);
	}
}

void ACommonAbility::HandleTriggerReleasedEvent(const FTriggerEventPayload& TriggeredEventPayload)
{
	UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::RequestingStart);

	// If there's a Trigger with no Activation (e.g. Jump just plays a montage) then reset the ability
	if(!ActivationMechanism)
	{
		UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::Active);
		return;
	}
	
	if (TriggeredEventPayload.bMontageDrivesActivation)
	{
		return;
	}

	if (TriggeredEventPayload.bStartActivationImmediately)
	{
		// Pass some info from the Trigger to Activation (needed for things like charge-up weapons, throwing grenades with a predicted location, etc)
		ActivationMechanism->Activate(TriggeredEventPayload);
	}
	else
	{
		ActivationMechanism->Deactivate();
	}
}

void ACommonAbility::HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent)
{
	UGameplayTagComponent::AddTagToActor(this, CommonGameAbilityEvent::OnCooldown);
}

void ACommonAbility::HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent)
{
	UGameplayTagComponent::RemoveTagFromActor(this, CommonGameAbilityEvent::OnCooldown);

	// If it's a burst trigger (3-round burst machine gun), try to activate it immediately after it's cooldown
	// BurstTrigger shoots 1-2-3 fast Activation ticks, cools down, then fires again. This is what triggers the refiring after the cooldown.
	if (TriggerMechanism && TriggerMechanism->ShouldRetriggerAbilityAfterCooldown() &&
		UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameAbilityEvent::RequestingStart))
	{
		Internal_StartNormalAbility();
	}
}
