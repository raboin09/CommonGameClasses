// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonCharacter.h"

#include "Ability/CommonAbility.h"
#include "ActorComponent/AbilityComponent.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/CommonCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/ActorSystems/ActorTrackingSubsystem.h"
#include "Types/CommonCoreTypes.h"
#include "Utils/CommonCombatUtils.h"
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
	BPI_HandleTagAdded(TagAddedEventPayload);
}

void ACommonCharacter::HandleTagRemoved(const FGameplayTagRemovedEventPayload& TagRemovedEventPayload)
{
	BPI_HandleTagRemoved(TagRemovedEventPayload);
}

void ACommonCharacter::HandleDeath(const FActorDeathEventPayload& EventPayload)
{
	if (!IsAlive())
	{
		return;
	}
	UGameplayTagComponent::AddTagToActor(this, CommonStateTags::Dead);
	DetachFromControllerPendingDestroy();
	AbilityComponent->DestroyAbilities();
	BPI_HandleDeath();
	float DeathLifeSpan = DeathBuffer + UCommonCombatUtils::GetKnockbackRecoveryTime(EventPayload.HitReactEvent.DeathReactType);	
	SetLifeSpan(DeathLifeSpan + DeathBuffer);
}

void ACommonCharacter::InitTopDownMovementDefaults() const
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->bCanWalkOffLedges = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
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
	GetMesh()->SetCollisionResponseToChannel(COMMON_TRACE_ABILITY, ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(COMMON_OBJECT_TYPE_PROJECTILE, ECR_Block);
}
