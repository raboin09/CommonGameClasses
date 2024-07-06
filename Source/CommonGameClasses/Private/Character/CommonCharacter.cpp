// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonCharacter.h"

#include "Ability/CommonAbility.h"
#include "ActorComponent/AbilityComponent.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/CommonCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Systems/ActorTrackingSubsystem.h"
#include "Types/CommonCoreTypes.h"
#include "Utils/CommonCoreUtils.h"


ACommonCharacter ::ACommonCharacter(const FObjectInitializer& ObjectInitializer) : Super {ObjectInitializer.SetDefaultSubobjectClass<UCommonCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)}
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	EffectContainerComponent = CreateDefaultSubobject<UEffectContainerComponent>(TEXT("EffectContainerComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	CharacterAnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>(TEXT("CharacterAnimationComponent"));

	UCommonCharacterMovementComponent* CommonMoveComp = CastChecked<UCommonCharacterMovementComponent>(GetCharacterMovement());
	CommonMoveComp->GravityScale = 1.0f;
	CommonMoveComp->MaxAcceleration = 2400.0f;
	CommonMoveComp->BrakingFrictionFactor = 1.0f;
	CommonMoveComp->BrakingFriction = 6.0f;  
	CommonMoveComp->GroundFriction = 8.0f;  
	CommonMoveComp->BrakingDecelerationWalking = 1400.0f; 
	CommonMoveComp->bUseControllerDesiredRotation = false;
	CommonMoveComp->bOrientRotationToMovement = false;
	CommonMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	CommonMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	CommonMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	CommonMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	CommonMoveComp->SetCrouchedHalfHeight(65.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
	
	InitCapsuleDefaults();
	InitCharacterMeshDefaults();
}

void ACommonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
	if(!AbilityComponent)
	{
		return;
	}
	
	for(auto Ability : DefaultAbilities)
	{
		if(Ability.Value.IsNull())
		{
			continue;
		}
		
		AbilityComponent->AddAbilityFromClassInSlot(Ability.Value, Ability.Key);
		if(Ability.Key == CommonGameSlot::SlotMain)
		{
			AbilityComponent->SetCurrentEquippedSlot(CommonGameSlot::SlotMain);;
		}
	}
}

void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(UActorTrackingSubsystem* ActorTrackingSubsystem = UCommonCoreUtils::GetActorTrackingSubsystem(this))
	{
		ActorTrackingSubsystem->TryAddActorToTrackedArrays(this);	
	}
}

void ACommonCharacter::HandleTagAdded(const FGameplayTagAddedEventPayload& TagAddedEventPayload)
{
	K2_HandleTagAdded(TagAddedEventPayload);
}

void ACommonCharacter::HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload)
{
	K2_HandleTagRemoved(TagRemovedEventPayload);
}

void ACommonCharacter::HandleDeath()
{
	if (!IsAlive())
	{
		return;
	}
	UGameplayTagComponent::AddTagToActor(this, CommonGameState::Dead);
	DetachFromControllerPendingDestroy();
	AbilityComponent->DestroyAbilities();
	K2_HandleDeath();
}

void ACommonCharacter::InitCapsuleDefaults() const
{
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COMMON_TRACE_ABILITY, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COMMON_OBJECT_TYPE_PROJECTILE, ECR_Ignore);
}

void ACommonCharacter::InitCharacterMeshDefaults() const
{
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannels(ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(COMMON_TRACE_ABILITY, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COMMON_OBJECT_TYPE_PROJECTILE, ECR_Block);
}
