
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/MountManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/CommonPlayerController.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Utils/CommonCombatUtils.h"
#include "Utils/CommonCoreUtils.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	OwnerCharacter = nullptr;
	OwnerAnimInstance = nullptr;
	OwningTagComponent = nullptr;
	ControlRotation = FRotator();
	RagdollMeshLocation = FVector();
	CachedMeshOffset = FVector();
	CachedMeshRotation = FRotator();
}

void UCharacterAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(UGameplayTagComponent::ActorHasGameplayTag(OwnerCharacter.Get(), CommonStateTags::Ragdoll))
	{
		Internal_RagdollUpdate(DeltaTime);
	}
}

void UCharacterAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACommonCharacter>(GetOwner());
	if(!OwnerCharacter.IsValid())
	{
		return;
	}
	OwningTagComponent = OwnerCharacter->GetGameplayTagComponent();
	OwnerAnimInstance = OwnerCharacter->GetAnimInstance();
	
	if(OwnerAnimInstance.IsValid())
	{
		OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::HandleMontageEnded);
	}
	
	if(UHealthComponent* HealthComponent = OwnerCharacter->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->OnCurrentWoundHealthChanged().AddDynamic(this, &ThisClass::HandleCurrentWoundChangedEvent);
		HealthComponent->OnActorDeath().AddDynamic(this, &ThisClass::HandleActorDeathEvent);
	}

	if(GetMesh())
	{
		CachedMeshRotation = GetMesh()->GetRelativeRotation();	
	}
}

void UCharacterAnimationComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if(ACommonPlayerController* PlayerController = UCommonCoreUtils::GetCommonPlayerController(this))
	{
		PlayerController->OnCameraTypeChanged().AddUniqueDynamic(this, &ThisClass::HandleCameraTypeChanged);
	}
}

void UCharacterAnimationComponent::HandleMontageEnded(UAnimMontage* EndedMontage, bool bInterrupted)
{
	const FCharacterMontageEndedPayload CharacterMontageEndedPayload = FCharacterMontageEndedPayload(EndedMontage, GetCurrentPlayingMontage().Get(), bInterrupted);
	CharacterMontageEnded.Broadcast(CharacterMontageEndedPayload);
}

void UCharacterAnimationComponent::HandleCurrentWoundChangedEvent(const FCurrentWoundEventPayload& CurrentWoundEventPayload)
{
	if(!CurrentWoundEventPayload.bNaturalChange)
	{
		return;
	}
	Internal_TryStartCharacterKnockback(CurrentWoundEventPayload.DamageHitReactEvent, false);
}

void UCharacterAnimationComponent::HandleActorDeathEvent(const FActorDeathEventPayload& DeathEventPayload)
{
	Internal_TryStartCharacterKnockback(DeathEventPayload.HitReactEvent, true);
}

void UCharacterAnimationComponent::HandleCameraTypeChanged(const FCameraTypeChangedPayload& CameraTypeChangedPayload)
{
	switch(CameraTypeChangedPayload.NewCameraType)
	{
	case ECameraType::None:
	case ECameraType::FirstPerson:
	case ECameraType::TopDown:
		Internal_ChangeRotation(ERotationMethod::NONE);
		break;
	case ECameraType::ThirdPerson:
		Internal_ChangeRotation(ERotationMethod::SmoothedControlRotation);
		break;
	}
}

float UCharacterAnimationComponent::TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if(!OwnerCharacter.IsValid() || OwnerCharacter->GetCurrentMontage())
	{
		return -1.f;
	}
	return Internal_PlayMontage(AnimMontageData);
}

void UCharacterAnimationComponent::StopAnimMontage(UAnimMontage* Montage)
{
	if(!OwnerCharacter.IsValid())
	{
		return;
	}
	OwnerCharacter->StopAnimMontage(Montage);
}

float UCharacterAnimationComponent::ForcePlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if(!AnimMontageData.MontageToPlay.IsValid())
	{
		return -1.f;
	}
	StopAnimMontage();
	return Internal_PlayMontage(AnimMontageData);
}

void UCharacterAnimationComponent::SetAnimationOverlay(const FGameplayTag& NewOverlay)
{
	if(!OwnerCharacter.IsValid())
	{
		return;
	}
	// OwnerCharacter->SetOverlayMode(NewOverlay);
}

void UCharacterAnimationComponent::StartRagdolling()
{
	if(!OwnerCharacter.IsValid())
	{
		return;
	}
	UGameplayTagComponent::AddTagToActor(OwnerCharacter.Get(), CommonStateTags::Ragdoll);
	StopAnimMontage();
	CachedMeshOffset = GetMesh()->GetRelativeLocation();
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(CommonGameAnimation::NAME_Pelvis, true);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopActiveMovement();
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetComponentTickEnabled(true);
	OnCharacterRagdollingEvent().Broadcast(FCharacterRagdollEventPayload(true));
}

void UCharacterAnimationComponent::StopRagdolling()
{
	if(!OwnerCharacter.IsValid())
	{
		return;
	}
	UGameplayTagComponent::RemoveTagFromActor(OwnerCharacter.Get(), CommonStateTags::Ragdoll);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(CommonGameAnimation::NAME_Pelvis, false);
	GetMesh()->SetRelativeRotation(CachedMeshRotation);
	GetMesh()->SetRelativeLocation(CachedMeshOffset);
	SetComponentTickEnabled(false);
	OnCharacterRagdollingEvent().Broadcast(FCharacterRagdollEventPayload(false));
}

float UCharacterAnimationComponent::Internal_PlayMontage(const FAnimMontagePlayData& AnimMontagePlayData)
{	
	if(AnimMontagePlayData.MontageToPlay.IsValid())
	{
		return OwnerCharacter->PlayAnimMontage(AnimMontagePlayData.MontageToPlay.Get(), AnimMontagePlayData.PlayRate, AnimMontagePlayData.MontageSection);	
	}
	return 0.f;
}

void UCharacterAnimationComponent::Internal_ApplyRagdollKnockback(const FVector& Impulse, const float ImpulseScale, const FName BoneName, bool bVelocityChange)
{
	// TODO Add condition for knocking character off of mount on hit reactions
	if(const UMountManagerComponent* MountManager = GetOwner()->FindComponentByClass<UMountManagerComponent>())
	{
		if(MountManager->IsMounted())
		{
			return;
		}	
	}
	
	StartRagdolling();
	GetMesh()->AddImpulse(Impulse * ImpulseScale, BoneName, bVelocityChange);
}

void UCharacterAnimationComponent::Internal_TryCharacterKnockbackRecovery()
{
	if(!OwnerCharacter->IsAlive())
	{
		return;
	}
	
	if (LastRagdollVelocity.Size() > 10)
	{
		OwnerCharacter->GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, this, &ThisClass::Internal_TryCharacterKnockbackRecovery, .1f, false);
	}
	else
	{
		StopRagdolling();
	}
}

void UCharacterAnimationComponent::Internal_TryStartCharacterKnockback(const FDamageHitReactEvent& HitReactEvent, bool bIsDeathKnockback)
{
	EHitReactType HitReactType = bIsDeathKnockback ? HitReactEvent.DeathReactType : HitReactEvent.HitReactType;
	float ImpulseValue = UCommonCombatUtils::GetHitImpulseValue(HitReactType);
	if((!bIsDeathKnockback && ImpulseValue == 0.f) || UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonStateTags::Immovable))
	{
		return;
	}

	const float KnockdownDuration = UCommonCombatUtils::GetKnockbackRecoveryTime(HitReactType);
	const FName HitBoneName = UCommonCombatUtils::GetNearestValidBoneForImpact(HitReactEvent.HitResult.BoneName);
	switch(HitReactType) {
		case EHitReactType::Ragdoll_Tiny:
		case EHitReactType::Ragdoll_VeryLight:
		case EHitReactType::Ragdoll_Light:
		case EHitReactType::Ragdoll_Medium:
		case EHitReactType::Ragdoll_Heavy:
		case EHitReactType::Ragdoll_VeryHeavy:
		case EHitReactType::Ragdoll_Huge:
			Internal_ApplyRagdollKnockback(HitReactEvent.HitDirection, ImpulseValue, HitBoneName, false);
			if(!bIsDeathKnockback)
			{
				OwnerCharacter->GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, this, &ThisClass::Internal_TryCharacterKnockbackRecovery, KnockdownDuration, false);	
			}
			break;
		case EHitReactType::Launch_Tiny:
		case EHitReactType::Launch_VeryLight:
		case EHitReactType::Launch_Light:
		case EHitReactType::Launch_Medium:
		case EHitReactType::Launch_Heavy:
		case EHitReactType::Launch_VeryHeavy:
		case EHitReactType::Launch_Huge:
			Internal_TryStartLaunchKnockback(HitReactEvent, bIsDeathKnockback);
			break;
		default:
		case EHitReactType::None:
			break;
	}
}

void UCharacterAnimationComponent::Internal_TryStartLaunchKnockback(const FDamageHitReactEvent& HitReactEvent, bool bIsDeathKnockback)
{
	ACharacter* HitCharacter = Cast<ACharacter>(HitReactEvent.HitResult.GetActor());
	if(!HitCharacter)
	{
		return;
	}
	if(UGameplayTagComponent::ActorHasGameplayTag(HitCharacter, CommonStateTags::Immovable))
	{
		return;
	}
	if(UGameplayTagComponent::ActorHasGameplayTag(HitCharacter, CommonStateTags::Ragdoll))
	{
		return;
	}
	FVector LaunchDirection = FVector(HitReactEvent.HitDirection.X, HitReactEvent.HitDirection.Y, 0.f);
	LaunchDirection *= UCommonCombatUtils::GetHitImpulseValue(HitReactEvent.HitReactType);
	HitCharacter->LaunchCharacter(LaunchDirection, true, true);
}

FGameplayTag UCharacterAnimationComponent::Internal_GetHitDirectionTag(const FVector& OriginatingLocation) const
{
	const FVector& ActorLocation = OwnerCharacter->GetActorLocation();
	const float DistanceToFrontBackPlane = FVector::PointPlaneDist(OriginatingLocation, ActorLocation, OwnerCharacter->GetActorRightVector());
	const float DistanceToRightLeftPlane = FVector::PointPlaneDist(OriginatingLocation, ActorLocation, OwnerCharacter->GetActorForwardVector());
	
	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		if (DistanceToRightLeftPlane >= 0)
		{
			return CommonAnimationTags::HitReactFront;
		}
		return CommonAnimationTags::HitReactBack;
	}
	if (DistanceToFrontBackPlane >= 0)
	{
		return CommonAnimationTags::HitReactRight;
	}
	return CommonAnimationTags::HitReactLeft;
}

void UCharacterAnimationComponent::Internal_RagdollUpdate(float DeltaTime)
{
	USkeletalMeshComponent* OwnerMesh = GetMesh();
	if(!OwnerMesh)
	{
		return;
	}
	const FVector NewRagdollVel = OwnerMesh->GetPhysicsLinearVelocity(CommonGameAnimation::NAME_Pelvis);
	LastRagdollVelocity = (NewRagdollVel != FVector::ZeroVector || OwnerCharacter->IsLocallyControlled()) ? NewRagdollVel : LastRagdollVelocity / 2;

	RagdollMeshLocation = Internal_RagdollTraceGround();
	OwnerCharacter->SetActorLocation(RagdollMeshLocation - CachedMeshOffset);
}

FVector UCharacterAnimationComponent::Internal_RagdollTraceGround() const
{
	FVector TraceStart = GetMesh()->GetSocketLocation(CommonGameAnimation::NAME_Pelvis);
	FVector TraceEnd = TraceStart;
	TraceEnd.Z -= 100;
	FHitResult Hit;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter.Get());
	UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, IgnoreActors, EDrawDebugTrace::None, Hit, true);
	if(!Hit.bBlockingHit)
	{
		return TraceStart;
	}
	return Hit.Location;
}

void UCharacterAnimationComponent::Internal_ChangeRotation(ERotationMethod RotationMethod)
{
	switch(RotationMethod) {
		case ERotationMethod::NONE:
			OwnerCharacter.Get()->bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			break;
		case ERotationMethod::CharacterVelocity:
			OwnerCharacter.Get()->bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			break;
		case ERotationMethod::AbsoluteControlRotation:
			OwnerCharacter.Get()->bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			break;
		case ERotationMethod::SmoothedControlRotation:
			OwnerCharacter.Get()->bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			break;
		case ERotationMethod::AngleControlRotation:
			OwnerCharacter.Get()->bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			break;
	}
}
