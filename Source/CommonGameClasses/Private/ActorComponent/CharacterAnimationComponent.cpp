
#include "ActorComponent/CharacterAnimationComponent.h"

#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/MountManagerComponent.h"
#include "GameFramework/Character.h"
#include "Types/CommonCharacterAnimTypes.h"
#include "Utils/CommonCombatUtils.h"


UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwnerCharacter = nullptr;
	OwningTagComponent = nullptr;
}

void UCharacterAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(UGameplayTagComponent::ActorHasGameplayTag(OwnerCharacter, CommonGameState::Ragdoll))
	{
		Internal_RagdollUpdate();
	}
}

void UCharacterAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACommonCharacter>(GetOwner());
	if(!OwnerCharacter)
	{
		return;
	}
	OwningTagComponent = OwnerCharacter->GetGameplayTagComponent();
	if(UHealthComponent* HealthComponent = OwnerCharacter->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->OnCurrentWoundHealthChanged().AddDynamic(this, &UCharacterAnimationComponent::HandleCurrentWoundChangedEvent);
		HealthComponent->OnActorDeath().AddDynamic(this, &UCharacterAnimationComponent::HandleActorDeathEvent);
	}
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

float UCharacterAnimationComponent::TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if(!OwnerCharacter || OwnerCharacter->GetCurrentMontage())
	{
		return -1.f;
	}
	return OwnerCharacter->PlayAnimMontage(AnimMontageData.MontageToPlay);
}

void UCharacterAnimationComponent::StopAnimMontage(UAnimMontage* Montage)
{
	if(!OwnerCharacter)
	{
		return;
	}
	OwnerCharacter->StopAnimMontage(Montage);
}

float UCharacterAnimationComponent::ForcePlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if(!AnimMontageData.MontageToPlay)
	{
		return -1.f;
	}
	StopAnimMontage();
	return Internal_PlayMontage(AnimMontageData);
}

void UCharacterAnimationComponent::SetAnimationOverlay(const FGameplayTag& NewOverlay)
{
	if(!OwnerCharacter)
	{
		return;
	}
	OwnerCharacter->SetOverlayMode(NewOverlay);
}

void UCharacterAnimationComponent::StartRagdolling()
{
	if(!OwnerCharacter)
	{
		return;
	}
	OwningTagComponent->AddTag(CommonGameState::Ragdoll);
	OwnerCharacter->StartRagdolling();
}

void UCharacterAnimationComponent::StopRagdolling()
{
	if(!OwnerCharacter)
	{
		return;
	}
	OwningTagComponent->RemoveTag(CommonGameState::Ragdoll);
	OwnerCharacter->StopRagdolling();
}

float UCharacterAnimationComponent::Internal_PlayMontage(const FAnimMontagePlayData& AnimMontagePlayData)
{
	if (!AnimMontagePlayData.MontageToPlay)
	{
		return 0.f;
	}	
	AnimMontagePlayData.MontageToPlay->bEnableAutoBlendOut = AnimMontagePlayData.bShouldBlendOut;
	return OwnerCharacter->PlayAnimMontage(AnimMontagePlayData.MontageToPlay, AnimMontagePlayData.PlayRate, AnimMontagePlayData.MontageSection);
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
	OwnerCharacter->GetMesh()->AddImpulse(Impulse * ImpulseScale, BoneName, bVelocityChange);
}

void UCharacterAnimationComponent::Internal_TryCharacterKnockbackRecovery()
{
	if(!OwnerCharacter->IsAlive())
	{
		return;
	}
	
	if (LastRagdollVelocity.Size() > 100)
	{
		OwnerCharacter->GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, this, &UCharacterAnimationComponent::Internal_TryCharacterKnockbackRecovery, .1f, false);
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
		OwnerCharacter->GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, this, &UCharacterAnimationComponent::Internal_TryCharacterKnockbackRecovery, KnockdownDuration, false);	
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
	
	if(HitReactEvent.HitReactType == EHitReactType::HitReact_Chainsaw || HitReactEvent.DeathReactType == EHitReactType::HitReact_Chainsaw)
	{
		PlayData.MontageToPlay = OwnerCharacter->K2_GetHitReactAnimation(CommonGameAnimation::HitReactChainsaw);
		PlayData.bShouldBlendOut = false;
	} else
	{
		PlayData.MontageToPlay = OwnerCharacter->K2_GetHitReactAnimation(Internal_GetHitDirectionTag(HitReactEvent.HitDirection));
		PlayData.bShouldBlendOut = true;
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

void UCharacterAnimationComponent::Internal_RagdollUpdate()
{
	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	if(!OwnerMesh)
	{
		return;
	}
	const FVector NewRagdollVel = OwnerMesh->GetPhysicsLinearVelocity("root");
	LastRagdollVelocity = (NewRagdollVel != FVector::ZeroVector || OwnerCharacter->IsLocallyControlled()) ? NewRagdollVel : LastRagdollVelocity / 2;
}