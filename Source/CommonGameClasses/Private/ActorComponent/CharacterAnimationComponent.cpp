
#include "ActorComponent/CharacterAnimationComponent.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/MountManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Utils/CommonCombatUtils.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	OwnerCharacter = nullptr;
	OwnerAnimInstance = nullptr;
	OwningTagComponent = nullptr;
}

void UCharacterAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(UGameplayTagComponent::ActorHasGameplayTag(OwnerCharacter.Get(), CommonGameState::Ragdoll))
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
	Internal_TryStartCharacterKnockback(CurrentWoundEventPayload.DamageHitReactEvent, true);
}

void UCharacterAnimationComponent::HandleActorDeathEvent(const FActorDeathEventPayload& DeathEventPayload)
{
	Internal_TryStartCharacterKnockback(DeathEventPayload.HitReactEvent, false);
}

float UCharacterAnimationComponent::HandleMontageLoadedEvent(TSoftObjectPtr<UAnimMontage> LoadedAnimMontage)
{
	if(!CachedMontageData.Contains(LoadedAnimMontage))
	{
		return -1.f;
	}
	const FAnimMontagePlayData& AnimMontagePlayData = CachedMontageData[LoadedAnimMontage];
	CachedMontageData.Remove(LoadedAnimMontage);
	if(AnimMontagePlayData.bForcePlay)
	{
		return ForcePlayAnimMontage(AnimMontagePlayData);
	}
	return TryPlayAnimMontage(AnimMontagePlayData);
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
	// TODO ALS
	// OwnerCharacter->SetOverlayMode(NewOverlay);
}

void UCharacterAnimationComponent::StartRagdolling()
{
	if(!OwnerCharacter.IsValid())
	{
		return;
	}
	UGameplayTagComponent::AddTagToActor(OwnerCharacter.Get(), CommonGameState::Ragdoll);
	StopAnimMontage();
	CachedMeshOffset = GetMesh()->GetRelativeLocation();
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopActiveMovement();
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetComponentTickEnabled(true);
}

void UCharacterAnimationComponent::StopRagdolling()
{
	if(!OwnerCharacter.IsValid())
	{
		return;
	}
	UGameplayTagComponent::RemoveTagFromActor(OwnerCharacter.Get(), CommonGameState::Ragdoll);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", false);
	SetComponentTickEnabled(false);
}

float UCharacterAnimationComponent::Internal_PlayMontage(const FAnimMontagePlayData& AnimMontagePlayData)
{	
	if(AnimMontagePlayData.MontageToPlay.IsValid())
	{
		return OwnerCharacter->PlayAnimMontage(AnimMontagePlayData.MontageToPlay.Get(), AnimMontagePlayData.PlayRate, AnimMontagePlayData.MontageSection);	
	}
	return 0.f;
}

void UCharacterAnimationComponent::Internal_ApplyCharacterKnockback(const FVector& Impulse, const float ImpulseScale, const FName BoneName, bool bVelocityChange)
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

void UCharacterAnimationComponent::Internal_TryStartCharacterKnockback(const FDamageHitReactEvent& HitReactEvent, bool bShouldRecoverFromKnockback)
{	
	float ImpulseValue = UCommonCombatUtils::GetHitImpulseValue(HitReactEvent.HitReactType);
	if(!bShouldRecoverFromKnockback)
	{
		ImpulseValue = UCommonCombatUtils::GetHitImpulseValue(EHitReactType::Knockback_VeryLight);
	}
	
	else if((bShouldRecoverFromKnockback && ImpulseValue == 0.f) || UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameState::Immovable))
	{
		return;
	}
	
	const float KnockdownDuration = UCommonCombatUtils::GetKnockbackRecoveryTime(HitReactEvent.HitReactType);
	const FName HitBoneName = UCommonCombatUtils::GetNearestValidBoneForImpact(HitReactEvent.HitResult.BoneName);
	Internal_ApplyCharacterKnockback(HitReactEvent.HitDirection, ImpulseValue, HitBoneName, false);
	if(bShouldRecoverFromKnockback)
	{
		OwnerCharacter->GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, this, &ThisClass::Internal_TryCharacterKnockbackRecovery, KnockdownDuration, false);	
	}
}

void UCharacterAnimationComponent::Internal_TryPlayHitReact(const FDamageHitReactEvent& HitReactEvent)
{
	if(!OwnerCharacter->IsAlive())
	{
		return;
	}

	StopAnimMontage();
	FAnimMontagePlayData PlayData;
	PlayData.MontageSection = FName();
	PlayData.PlayRate = 1.f;
	
	if(HitReactEvent.HitReactType == EHitReactType::HitReact_Special1 || HitReactEvent.DeathReactType == EHitReactType::HitReact_Special1)
	{
		PlayData.MontageToPlay = OwnerCharacter->K2_GetHitReactAnimation(CommonGameAnimation::HitReactSpecial1).Get();
	} else
	{
		PlayData.MontageToPlay = OwnerCharacter->K2_GetHitReactAnimation(Internal_GetHitDirectionTag(HitReactEvent.HitDirection)).Get();
	}
	
	ForcePlayAnimMontage(PlayData);
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
			return CommonGameAnimation::HitReactFront;
		}
		return CommonGameAnimation::HitReactBack;
	}
	if (DistanceToFrontBackPlane >= 0)
	{
		return CommonGameAnimation::HitReactRight;
	}
	return CommonGameAnimation::HitReactLeft;
}

void UCharacterAnimationComponent::Internal_RagdollUpdate(float DeltaTime)
{
	USkeletalMeshComponent* OwnerMesh = GetMesh();
	if(!OwnerMesh)
	{
		return;
	}
	const FVector NewRagdollVel = OwnerMesh->GetPhysicsLinearVelocity("pelvis");
	LastRagdollVelocity = (NewRagdollVel != FVector::ZeroVector || OwnerCharacter->IsLocallyControlled()) ? NewRagdollVel : LastRagdollVelocity / 2;

	RagdollMeshLocation = Internal_RagdollTraceGround();
	OwnerCharacter->GetCapsuleComponent()->SetWorldLocation(RagdollMeshLocation - CachedMeshOffset);
}

FVector UCharacterAnimationComponent::Internal_RagdollTraceGround() const
{
	FVector TraceStart = GetMesh()->GetSocketLocation("pelvis");
	FVector TraceEnd = TraceStart;
	TraceEnd.Z -= 100;
	FHitResult Hit;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter.Get());
	UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoreActors, EDrawDebugTrace::None, Hit, true);
	if(!Hit.bBlockingHit)
	{
		return TraceStart;
	}
	return Hit.Location;
}
