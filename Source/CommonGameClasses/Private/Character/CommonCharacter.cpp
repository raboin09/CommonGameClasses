// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CommonCharacter.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Animation/CharacterAnimationComponent.h"
#include "API/Mountable.h"
#include "Components/CapsuleComponent.h"
#include "Core/CommonGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utils/CommonCombatUtils.h"


ACommonCharacter::ACommonCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	GameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));
	EffectContainerComponent = CreateDefaultSubobject<UEffectContainerComponent>(TEXT("EffectContainerComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	CharacterAnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>(TEXT("CharacterAnimationComponent"));
}

void ACommonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UGameplayTagComponent::AddTagsToActor(this, DefaultGameplayTags);
}

void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(!AbilityComponent)
	{
		return;
	}
	
	for(auto Ability : DefaultAbilities)
	{
		AbilityComponent->AddAbilityFromClassInSlot(Ability.Value, Ability.Key);		
	}
	ACommonGameMode::TryAddActorToTrackedArrays(this);
}

void ACommonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(IsRagdoll())
	{
		Internal_RagdollUpdate();
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

void ACommonCharacter::K2_OnDeath_Implementation()
{
	if (!IsAlive())
	{
		return;
	}
	UGameplayTagComponent::AddTagToActor(this, CommonGameState::Dead);
	GetMesh()->SetRenderCustomDepth(false);
	DetachFromControllerPendingDestroy();
	SetLifeSpan(5.f);
}

void ACommonCharacter::AssignNewMountable(UObject* InMountableObject, const FHitResult& InHitResult)
{
	if(IMountable* Mount = Cast<IMountable>(InMountableObject))
	{
		Mount->OccupyMount(this, FVector::ZeroVector, InHitResult.ImpactNormal);
		TScriptInterface<IMountable> NewCover;
		NewCover.SetObject(InMountableObject);
		NewCover.SetInterface(Mount);
		CurrentMount = NewCover;
	}
}

void ACommonCharacter::RagdollStart()
{
	DefVisBasedTickOp = GetMesh()->VisibilityBasedAnimTickOption;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	TargetRagdollLocation = GetMesh()->GetSocketLocation(NAME_PELVIS);
	bPreRagdollURO = GetMesh()->bEnableUpdateRateOptimizations;
	GetMesh()->bEnableUpdateRateOptimizations = false;
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(NAME_PELVIS, true, true);
	Internal_StopAllAnimMontages();
}

void ACommonCharacter::RagdollEnd()
{
	GetMesh()->VisibilityBasedAnimTickOption = DefVisBasedTickOp;
	GetMesh()->bEnableUpdateRateOptimizations = bPreRagdollURO;
	GetMesh()->bOnlyAllowAutonomousTickPose = false;
	if (bRagdollOnGround)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		FAnimMontagePlayData PlayData;
		PlayData.MontageSection = FName();
		PlayData.PlayRate = 1.f;
		PlayData.MontageToPlay = K2_GetGetUpAnimation(bRagdollFaceUp);
		PlayData.bShouldBlendOut = false;
		ForcePlayAnimMontage(PlayData);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetCharacterMovement()->Velocity = LastRagdollVelocity;
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
}

void ACommonCharacter::Internal_RagdollUpdate()
{
	const FVector NewRagdollVel = GetMesh()->GetPhysicsLinearVelocity(NAME_ROOT);
	LastRagdollVelocity = (NewRagdollVel != FVector::ZeroVector || IsLocallyControlled())
							  ? NewRagdollVel
							  : LastRagdollVelocity / 2;
	const float SpringValue = FMath::GetMappedRangeValueClamped<float, float>({0.0f, 1000.0f}, {0.0f, 25000.0f},
																LastRagdollVelocity.Size());
	GetMesh()->SetAllMotorsAngularDriveParams(SpringValue, 0.0f, 0.0f, false);
	const bool bEnableGrav = LastRagdollVelocity.Z > -4000.0f;
	GetMesh()->SetEnableGravity(bEnableGrav);
	SetActorLocationDuringRagdoll();
}

void ACommonCharacter::SetActorLocationDuringRagdoll()
{
	TargetRagdollLocation = GetMesh()->GetSocketLocation(NAME_PELVIS);
	const FRotator PelvisRot = GetMesh()->GetSocketRotation(NAME_PELVIS);
	bRagdollFaceUp = PelvisRot.Roll < 0.0f;
	const FRotator TargetRagdollRotation(0.0f, bRagdollFaceUp ? PelvisRot.Yaw - 180.0f : PelvisRot.Yaw, 0.0f);
	const FVector TraceVect(TargetRagdollLocation.X, TargetRagdollLocation.Y,
	                        TargetRagdollLocation.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	World->LineTraceSingleByChannel(HitResult, TargetRagdollLocation, TraceVect,ECC_Visibility, Params);

	bRagdollOnGround = HitResult.IsValidBlockingHit();
	FVector NewRagdollLoc = TargetRagdollLocation;

	if (bRagdollOnGround)
	{
		const float ImpactDistZ = FMath::Abs(HitResult.ImpactPoint.Z - HitResult.TraceStart.Z);
		NewRagdollLoc.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - ImpactDistZ + 2.0f;
	}
	SetActorLocationAndRotation(bRagdollOnGround ? NewRagdollLoc : TargetRagdollLocation, TargetRagdollRotation);
}

void ACommonCharacter::Internal_ApplyCharacterKnockback(const FVector& Impulse, const float ImpulseScale, const FName BoneName, bool bVelocityChange)
{
	// TODO Add condition for knocking character off of mount on hit reactions
	if(IsMounted())
	{
		return;
	}
	RagdollStart();
	GetMesh()->AddImpulse(Impulse * ImpulseScale, BoneName, bVelocityChange);
}

void ACommonCharacter::Internal_TryStartCharacterKnockback(const FDamageHitReactEvent& HitReactEvent, bool bShouldRecoverFromKnockback)
{	
	float ImpulseValue = UCommonCombatUtils::GetHitImpulseValue(HitReactEvent.HitReactType);
	if(!bShouldRecoverFromKnockback)
	{
		ImpulseValue = UCommonCombatUtils::GetHitImpulseValue(EHitReactType::Knockback_VeryLight);
	}
	else if(bShouldRecoverFromKnockback && ImpulseValue == 0.f)
	{
		return;
	}
	const float KnockdownDuration = UCommonCombatUtils::GetKnockbackRecoveryTime(HitReactEvent.HitReactType);
	const FName HitBoneName = UCommonCombatUtils::GetNearestValidBoneForImpact(HitReactEvent.HitResult.BoneName);
	Internal_ApplyCharacterKnockback(HitReactEvent.HitDirection, ImpulseValue, HitBoneName, false);
	if(bShouldRecoverFromKnockback)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, this, &ACommonCharacter::Internal_TryCharacterKnockbackRecovery, KnockdownDuration, false);	
	}
}

void ACommonCharacter::Internal_TryCharacterKnockbackRecovery()
{
	if(!IsAlive())
	{
		return;
	}
	
	if (LastRagdollVelocity.Size() > 100)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, this, &ACommonCharacter::Internal_TryCharacterKnockbackRecovery, .1f, false);
	}
	else
	{
		RagdollEnd();
	}
}

void ACommonCharacter::Internal_TryPlayHitReact(const FDamageHitReactEvent& HitReactEvent)
{
	if(!IsAlive())
	{
		return;
	}

	Internal_StopAllAnimMontages();
	FAnimMontagePlayData PlayData;
	PlayData.MontageSection = FName();
	PlayData.PlayRate = 1.f;
	
	if(HitReactEvent.HitReactType == EHitReactType::HitReact_Chainsaw || HitReactEvent.DeathReactType == EHitReactType::HitReact_Chainsaw)
	{
		PlayData.MontageToPlay = K2_GetHitReactAnimation(CommonGameAnimation::HitReactChainsaw);
		PlayData.bShouldBlendOut = false;
	} else
	{
		PlayData.MontageToPlay = K2_GetHitReactAnimation(Internal_GetHitDirectionTag(HitReactEvent.HitDirection));
		PlayData.bShouldBlendOut = true;
	}
	
	ForcePlayAnimMontage(PlayData);
}

FGameplayTag ACommonCharacter::Internal_GetHitDirectionTag(const FVector& OriginatingLocation) const
{
	const FVector& ActorLocation = GetActorLocation();
	const float DistanceToFrontBackPlane = FVector::PointPlaneDist(OriginatingLocation, ActorLocation, GetActorRightVector());
	const float DistanceToRightLeftPlane = FVector::PointPlaneDist(OriginatingLocation, ActorLocation, GetActorForwardVector());
	
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

void ACommonCharacter::Internal_StopAllAnimMontages() const
{
	GetMesh()->AnimScriptInstance->Montage_Stop(0.0f);
}

float ACommonCharacter::TryPlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if (!GetCurrentMontage() && AnimMontageData.MontageToPlay)
	{
		return Internal_PlayMontage(AnimMontageData);
	}
	return -1.f;
}

float ACommonCharacter::ForcePlayAnimMontage(const FAnimMontagePlayData& AnimMontageData)
{
	if(!AnimMontageData.MontageToPlay)
	{
		return -1.f;
	}
	UAnimMontage* CurrentMontage = GetCurrentMontage();
	StopAnimMontage(CurrentMontage);
	return Internal_PlayMontage(AnimMontageData);
}

void ACommonCharacter::ForceStopAnimMontage(UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
}

float ACommonCharacter::Internal_PlayMontage(const FAnimMontagePlayData& AnimMontagePlayData)
{
	if (!AnimMontagePlayData.MontageToPlay || !GetMesh())
	{
		return 0.f;
	}
	
	AnimMontagePlayData.MontageToPlay->bEnableAutoBlendOut = AnimMontagePlayData.bShouldBlendOut;
	if(AnimMontagePlayData.bForceInPlace)
	{
		UKismetSystemLibrary::PrintString(this, "Make ALS for in place CommonCharacter::309");
		// if(UCommonAnimInstance* BaseAnimInstance = Cast<UCommonAnimInstance>(GetMesh()->GetAnimInstance()))
		// {
		// 	const int32 SectionIndex = AnimMontagePlayData.MontageToPlay->GetSectionIndex(AnimMontagePlayData.MontageSection);
		// 	BaseAnimInstance->DisableRootMotionModeForDuration(AnimMontagePlayData.MontageToPlay->GetSectionLength(SectionIndex));
		// }
	}	
	return PlayAnimMontage(AnimMontagePlayData.MontageToPlay, AnimMontagePlayData.PlayRate, AnimMontagePlayData.MontageSection);
}