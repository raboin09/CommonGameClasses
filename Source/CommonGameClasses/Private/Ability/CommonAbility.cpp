#include "Ability/CommonAbility.h"
#include "Ability/CooldownMechanismImpl.h"
#include "API/Ability/ActivationMechanism.h"
#include "API/Ability/CooldownMechanism.h"
#include "API/Ability/CostMechanism.h"
#include "API/Ability/TriggerMechanism.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Types/CoreTypes.h"
#include "Types/TagTypes.h"
#include "Utils/WorldUtils.h"

ACommonAbility::ACommonAbility()
{
	AbilityRoot = CreateDefaultSubobject<USphereComponent>(TEXT("SphereRoot"));
	SetRootComponent(AbilityRoot);
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
			Internal_StartNormalAbility();
			return true;
		}		
		return false;
	}

	if(UGameplayTagComponent::ActorHasGameplayTag(this, TAG_ABILITY_ON_COOLDOWN))
	{
		COMMON_PRINTSCREEN("On cooldown")
		return false;
	}
	
	if(!TriggerMechanism)
	{
		// If there's no trigger or activation, is this really an ability?
		if(!ActivationMechanism)
		{
			check(false)
			return false;
		}
		
		// Activate instantly, no trigger exists
		UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_REQUESTING_START);
		ActivationMechanism->Activate();
		return true;
	}

	// It's a standard ability with a Trigger, Activation, and (maybe) a Cost so proceed to the normal activation sequence
	return Internal_StartNormalAbility();
}

void ACommonAbility::CommitAbility()
{
	if(UGameplayTagComponent::ActorHasGameplayTag(this, TAG_ABILITY_REQUESTING_START))
	{
		UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_COMMITTED);	
	}
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
	CooldownMechanism.SetInterface(Cast<ICooldownMechanism>(Cooldown));
	CooldownMechanism.SetObject(Cooldown);
}

void ACommonAbility::SetCostMechanism(const TSubclassOf<AActor> InCostClass)
{
	APawn* CharOwner = Cast<APawn>(GetOwner());
	check(CharOwner)	
	//AI and non-Pawns don't use resources
	if(!CharOwner || !CharOwner->IsPlayerControlled())
	{
		return;
	}
	
	UObject* TempObj = Internal_CreateNewMechanism(InCostClass, UCostMechanism::StaticClass());
	if(!TempObj)
	{
		return;
	}
	CostMechanism.SetInterface(Cast<ICostMechanism>(TempObj));
	CostMechanism.SetObject(TempObj);
	CostMechanism->InitCostMechanism(CharOwner);
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

void ACommonAbility::BeginPlay()
{
	Super::BeginPlay();
	SetActivationMechanism(ActivationMechanismClass);
	SetCooldownMechanism();
	SetCostMechanism(CostMechanismClass);
	SetTriggerMechanism(TriggerMechanismClass);
	Internal_BindMechanismEventsToAbility();
}

bool ACommonAbility::Internal_StartNormalAbility()
{
	if(UGameplayTagComponent::ActorHasGameplayTag(this, TAG_ABILITY_COMMITTED))
	{
		return false;
	}
	
	// If no costs are required, press trigger instantly. No need for resources.
	if(!CostMechanism)
	{
		UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_REQUESTING_START);
		TriggerMechanism->PressTrigger();
		return true;
	}

	// Golden path if/else
	if(CostMechanism->CanConsumeResource())
	{
		UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_REQUESTING_START);
		if(!CostMechanism->TryReserveResource())
		{
			return false;
		}
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
	AActor* SpawnedActor = UWorldUtils::SpawnActorToWorld_Deferred<AActor>(GetWorld(), InMechanismClass, this, GetInstigator());
	UWorldUtils::FinishSpawningActor_Deferred(SpawnedActor, FTransform());
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
		TriggerMechanism->OnTriggerActivated().AddUObject(this, &ACommonAbility::HandleTriggerActivationEvent);
		TriggerMechanism->OnTriggerDeactivated().AddUObject(this, &ACommonAbility::HandleTriggerDeactivationEvent);
	}

	if(ActivationMechanism)
	{
		ActivationMechanism->OnActivation().AddUObject(this, &ACommonAbility::HandleAbilityActivationEvent);
		ActivationMechanism->OnDeactivation().AddUObject(this, &ACommonAbility::HandleAbilityDeactivationEvent);
	}
}

void ACommonAbility::HandleAbilityDeactivationEvent(const FAbilityDeactivationEventPayload& AbilityDeactivationEventPayload)
{
}

void ACommonAbility::HandleAbilityActivationEvent(const FAbilityActivationEventPayload& AbilityActivationEventPayload)
{
	if(CostMechanism)
	{
		CostMechanism->ConsumeResource();
	}

	// Some activation events (burst fire) only need to consume the resource and don't start cooldowns until after burst fire is over
	if(CooldownMechanism && AbilityActivationEventPayload.bStartCooldownImmediately)
	{
		CooldownMechanism->StartCooldownTimer();
	}
}

void ACommonAbility::HandleTriggerActivationEvent(const FTriggerActivatedEventPayload& AbilityTriggeredEventPayload) const
{
	if(!ActivationMechanism)
	{
		return;
	}

	// Montage-based activation, so don't activate until notified
	if(!AbilityTriggeredEventPayload.bDoesTriggerControlActivationDirectly)
	{
		return;
	}	
	ActivationMechanism->Activate();
}

void ACommonAbility::HandleTriggerDeactivationEvent(const FTriggerDeactivatedEventPayload& AbilityTriggeredEventPayload)
{
	if(!ActivationMechanism || !AbilityTriggeredEventPayload.bDoesTriggerControlActivationDirectly)
	{
		return;
	}
	UGameplayTagComponent::RemoveTagFromActor(this, TAG_ABILITY_REQUESTING_START);
	UGameplayTagComponent::RemoveTagFromActor(this, TAG_ABILITY_COMMITTED);	
	ActivationMechanism->Deactivate();
}

void ACommonAbility::HandleCooldownStarted(const FCooldownStartedEventPayload& AbilityCooldownStartedEvent)
{
	UGameplayTagComponent::AddTagToActor(this, TAG_ABILITY_ON_COOLDOWN);
}

void ACommonAbility::HandleCooldownEnded(const FCooldownEndedEventPayload& AbilityCooldownEndedEvent)
{
	UGameplayTagComponent::RemoveTagFromActor(this, TAG_ABILITY_ON_COOLDOWN);
	
	// If it's a burst trigger (machine gun), try to activate it immediately
	if(TriggerMechanism && TriggerMechanism->ShouldRetriggerAbilityAfterCooldown())
	{
		Internal_StartNormalAbility();	
	}
}