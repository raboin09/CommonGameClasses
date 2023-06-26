#include "Ability/CommonAbility.h"
#include "Ability/CooldownMechanismImpl.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "API/Ability/ActivationMechanism.h"
#include "API/Ability/CooldownMechanism.h"
#include "API/Ability/TriggerMechanism.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Types/CommonTagTypes.h"
#include "API/Ability/ResourceContainer.h"
#include "GameFramework/PlayerState.h"
#include "Types/CommonAbilityTypes.h"
#include "Types/CommonCoreTypes.h"
#include "Utils/CommonWorldUtils.h"

ACommonAbility::ACommonAbility()
{
	AbilityRoot = CreateDefaultSubobject<USphereComponent>(TEXT("SphereRoot"));
	SetRootComponent(AbilityRoot);

	AbilitySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AbilitySkeletalMesh"));
	InitWeaponMesh(AbilitySkeletalMesh);

	AbilityStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AbilityStaticMesh"));
	InitWeaponMesh(AbilityStaticMesh);
}

void ACommonAbility::InitWeaponMesh(UMeshComponent* InMeshComp) const
{
	if(!InMeshComp)
	{
		return;
	}
	
	InMeshComp->bReceivesDecals = false;
	InMeshComp->CastShadow = true;
	InMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	InMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	InMeshComp->SetCollisionResponseToChannel(COMMON_OBJECT_TYPE_PROJECTILE, ECR_Ignore);
	InMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	InMeshComp->SetupAttachment(AbilityRoot);
}


void ACommonAbility::BeginPlay()
{
	Super::BeginPlay();
	OwningPawn = GetInstigator();
	SetCooldownMechanism();
	SetActivationMechanism(ActivationMechanismClass);
	SetResourceContainerObject();
	SetTriggerMechanism(TriggerMechanismClass);
	Internal_BindMechanismEventsToAbility();
	if(!OwningPawn)
	{
		return;
	}
	OwningAbilityComponent = OwningPawn->FindComponentByClass<UAbilityComponent>();
}

bool ACommonAbility::TryStartAbility()
{
	if(UGameplayTagComponent::ActorHasGameplayTag(this, TAG_ABILITY_REQUESTING_START))
	{
		return false;
	}
	
	if(UGameplayTagComponent::ActorHasGameplayTag(this, TAG_ABILITY_COMBO_WINDOW_ENABLED))
	{
		if(TriggerMechanism && ActivationMechanism)
		{
			UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_COMBO_ACTIVATED);
			UGameplayTagComponent::RemoveTagFromActor(this, TAG_ABILITY_COMBO_WINDOW_ENABLED);
			return Internal_StartNormalAbility();
		}		
		return false;
	}
	
	if(UGameplayTagComponent::ActorHasGameplayTag(this, TAG_ABILITY_ON_COOLDOWN))
	{
		return false;
	}
	
	if(!TriggerMechanism)
	{
		// If there's no trigger or activation, is this really an ability?
		if(!ActivationMechanism)
		{
			return false;
		}
		
		// Activate instantly, no trigger exists
		UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_REQUESTING_START);
		ActivationMechanism->Activate(FTriggerEventPayload());
		return true;
	}

	// It's a standard ability with a Trigger, Activation, and (maybe) a Cost so proceed to the normal activation sequence
	return Internal_StartNormalAbility();
}

bool ACommonAbility::TryEndAbility()
{	
	if(!TriggerMechanism)
	{
		return true;
	}
	TriggerMechanism->ReleaseTrigger();
	return true;
}

void ACommonAbility::SetTriggerMechanism(const TSubclassOf<AActor> InTriggerClass)
{
	UObject* TempObj = Internal_CreateNewMechanism(InTriggerClass, UTriggerMechanism::StaticClass());
	if(!TempObj)
	{
		return;
	}
	TriggerMechanism.SetInterface(Cast<ITriggerMechanism>(TempObj));
	TriggerMechanism.SetObject(TempObj);
}

void ACommonAbility::SetCooldownMechanism()
{
	UCooldownMechanismImpl* SpawnedActor = NewObject<UCooldownMechanismImpl>(this);
	if(!SpawnedActor)
	{
		return;
	}
	CooldownMechanism.SetInterface(Cast<ICooldownMechanism>(SpawnedActor));
	CooldownMechanism.SetObject(SpawnedActor);
}

void ACommonAbility::SetResourceContainerObject()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in this ability or a component contained in this ability
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(ResourceContainerLocation == EResourceContainerLocation::Ability)
	{
		Internal_SetResourceContainerToObject(this);
		return;
	}
	if(ResourceContainerLocation == EResourceContainerLocation::AbilityComponent)
	{
		Internal_SetResourceContainerToComponent(this);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in Instigator or a component contained in Instigator
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(!OwningPawn)
	{
		return;
	}
	
	if(ResourceContainerLocation == EResourceContainerLocation::Instigator)
	{
		Internal_SetResourceContainerToObject(this);
		return;
	}
	if(ResourceContainerLocation == EResourceContainerLocation::InstigatorComponent)
	{
		Internal_SetResourceContainerToComponent(this);
		return;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in Instigator's Controller or a component contained in Instigator's Controller
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(ResourceContainerLocation == EResourceContainerLocation::PlayerController && OwningPawn->Controller)
	{
		Internal_SetResourceContainerToObject(OwningPawn->Controller);
		return;
	}
	if(ResourceContainerLocation == EResourceContainerLocation::PlayerControllerComponent && OwningPawn->Controller)
	{
		Internal_SetResourceContainerToComponent(OwningPawn->Controller);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Resource is located in Instigator's PlayerState or a component contained in Instigator's PlayerState
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(ResourceContainerLocation == EResourceContainerLocation::PlayerState && OwningPawn->GetPlayerState())
	{
		Internal_SetResourceContainerToObject(OwningPawn->GetPlayerState());
	}
	if(ResourceContainerLocation == EResourceContainerLocation::PlayerStateComponent && OwningPawn->GetPlayerState())
	{
		Internal_SetResourceContainerToComponent(OwningPawn->GetPlayerState());
	}	
}

void ACommonAbility::Internal_SetResourceContainerToObject(UObject* ContainerObject)
{
	ResourceContainer.SetInterface(Cast<IResourceContainer>(ContainerObject));
	ResourceContainer.SetObject(ContainerObject);
}

void ACommonAbility::Internal_SetResourceContainerToComponent(const AActor* PotentialActor)
{
	if(!PotentialActor)
		return;
	Internal_SetResourceContainerToObject(PotentialActor->GetComponentByClass(ResourceContainerClass));
}

void ACommonAbility::SetActivationMechanism(const TSubclassOf<AActor> InActivationClass)
{
	UObject* TempObj = Internal_CreateNewMechanism(InActivationClass, UActivationMechanism::StaticClass());
	if(!TempObj)
	{
		return;
	}
	ActivationMechanism.SetInterface(Cast<IActivationMechanism>(TempObj));
	ActivationMechanism.SetObject(TempObj);
}

bool ACommonAbility::Internal_StartNormalAbility()
{	
	// If no costs are required, press trigger instantly. No need for resources.
	if(!ResourceContainer)
	{
		UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_REQUESTING_START);
		TriggerMechanism->PressTrigger();
		return true;
	}

	// Golden path if/else
	if(ResourceContainer->TrySpendResource(ResourceCost))
	{
		UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_REQUESTING_START);
		TriggerMechanism->PressTrigger();
		return true;
	}
	return false;
}

AActor* ACommonAbility::Internal_CreateNewMechanism(const TSubclassOf<AActor> InMechanismClass, const UClass* InterfaceClass)
{
	if(!InMechanismClass || !InterfaceClass)
	{
		return nullptr;
	}

	if(!InMechanismClass->ImplementsInterface(InterfaceClass))
	{
		return nullptr;
	}
	AActor* SpawnedActor = UCommonWorldUtils::SpawnActorToPersistentWorld_Deferred<AActor>(InMechanismClass, this, OwningPawn);
	UCommonWorldUtils::FinishSpawningActor_Deferred(SpawnedActor, FTransform());
	return SpawnedActor;
}

void ACommonAbility::Internal_BindMechanismEventsToAbility()
{
	if(CooldownMechanism)
	{
		CooldownMechanism->OnCooldownTimerStarted().AddUObject(this, &ACommonAbility::HandleCooldownStarted);
		CooldownMechanism->OnCooldownTimerEnded().AddUObject(this, &ACommonAbility::HandleCooldownEnded);
	}
	
	if(TriggerMechanism)
	{		
		TriggerMechanism->OnTriggerPressed().AddUObject(this, &ACommonAbility::HandleTriggerPressedEvent);
		TriggerMechanism->OnTriggerReleased().AddUObject(this, &ACommonAbility::HandleTriggerReleasedEvent);
	}

	if(ActivationMechanism)
	{
		ActivationMechanism->OnActivation().AddUObject(this, &ACommonAbility::HandleAbilityActivationEvent);
		ActivationMechanism->OnDeactivation().AddUObject(this, &ACommonAbility::HandleAbilityDeactivationEvent);
	}
}

void ACommonAbility::HandleAbilityDeactivationEvent(const FAbilityDeactivationEventPayload& AbilityDeactivationEventPayload) const
{
	
}

void ACommonAbility::HandleAbilityActivationEvent(const FAbilityActivationEventPayload& AbilityActivationEventPayload) const
{
	// Some activation events don't start cooldowns until an external event happens (e.g. montage notifies)
	if(AbilityActivationEventPayload.bShouldStartCooldown && CooldownMechanism)
	{
		CooldownMechanism->StartCooldownTimer(CooldownDuration);
	}
}

void ACommonAbility::HandleTriggerPressedEvent(const FTriggerEventPayload& TriggeredEventPayload) const
{
	if(!ActivationMechanism)
	{
		return;
	}
	
	if(TriggeredEventPayload.bStartActivationImmediately)
	{
		// Pass some info from the Trigger to Activation (needed for things like charge-up weapons, throwing grenades with a predicted location, etc)
		ActivationMechanism->Activate(TriggeredEventPayload);
	} else
	{
		if(!OwningAbilityComponent || !TriggeredEventPayload.bMontageDrivesActivation)
		{
			return;
		}
		
		// Some activations don't start until an external event happens (e.g. montage notifies), so we store it
		// in the parent AbilityComponent
		FAwaitingActivationDetails AwaitingActivationDetails;
		AwaitingActivationDetails.AbilityTagComponent = GameplayTagComponent;
		AwaitingActivationDetails.MechanismAwaitingActivation = ActivationMechanism;
		AwaitingActivationDetails.ActivationLevel = TriggeredEventPayload.ActivationLevel;
		OwningAbilityComponent->SetMechanismAwaitingActivation(AwaitingActivationDetails);
	}
}

void ACommonAbility::HandleTriggerReleasedEvent(const FTriggerEventPayload& TriggeredEventPayload)
{
	UGameplayTagComponent::RemoveTagFromActor(this, TAG_ABILITY_REQUESTING_START);
	if(!ActivationMechanism || TriggeredEventPayload.bMontageDrivesActivation)
	{
		return;
	}

	if(TriggeredEventPayload.bStartActivationImmediately)
	{
		// Pass some info from the Trigger to Activation (needed for things like charge-up weapons, throwing grenades with a predicted location, etc)
		ActivationMechanism->Activate(TriggeredEventPayload);
	} else
	{
		ActivationMechanism->Deactivate();
	}
}

void ACommonAbility::HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent)
{
	UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_ON_COOLDOWN);
}

void ACommonAbility::HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent)
{
	UGameplayTagComponent::RemoveTagFromActor(this, TAG_ABILITY_ON_COOLDOWN);
	
	// If it's a burst trigger (3-round burst machine gun), try to activate it immediately after it's cooldown
	// BurstTrigger shoots 1-2-3 fast Activation ticks, cools down, then fires again. This is what triggers the refiring after the cooldown.
	if(TriggerMechanism && TriggerMechanism->ShouldRetriggerAbilityAfterCooldown() && UGameplayTagComponent::ActorHasGameplayTag(this, TAG_ABILITY_REQUESTING_START))
	{
		Internal_StartNormalAbility();	
	}
}