
#include "Ability/Activation/MeleeOverlapActivation.h"

#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonTagTypes.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInteractUtils.h"

void UMeleeOverlapActivation::InitActivationMechanism(UMeshComponent* OwnerMeshComponent)
{
	Super::InitActivationMechanism(OwnerMeshComponent);
	HitActors.Empty();
	check(MeshComponentRef)
	check(MeshComponentRef->DoesSocketExist(Socket_TraceStart));
	check(MeshComponentRef->DoesSocketExist(Socket_TraceEnd));
}

void UMeleeOverlapActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	Internal_StartAttack();
	FAbilityActivationEventPayload ActivationEventPayload;
	ActivationEventPayload.bShouldStartCooldown = false;
	AbilityActivationEvent.Broadcast(ActivationEventPayload);
}

void UMeleeOverlapActivation::Deactivate()
{
	Internal_StopAttack();
	FAbilityDeactivationEventPayload DeactivationEventPayload;
	AbilityDeactivationEvent.Broadcast(DeactivationEventPayload);
}

void UMeleeOverlapActivation::Internal_StartCollisionRaycastingTick()
{
	if(!MeshComponentRef)
	{
		return;
	}
	K2_HandleWeaponTraceStart();
	GetWorld()->GetTimerManager().SetTimer(Timer_Raycasting, this, &ThisClass::Internal_CheckForCollisionHit, TraceTickRate, true);
}

void UMeleeOverlapActivation::Internal_StopCollisionRaycastingTick()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer_Raycasting);
	K2_HandleWeaponTraceEnd();
}

void UMeleeOverlapActivation::Internal_CheckForCollisionHit()
{
	if(!MeshComponentRef || !UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameAbilityEvent::Activated))
	{
		Internal_StopCollisionRaycastingTick();
		return;
	}
	
	if(!bCanHitMultipleEnemies && HitActors.Num() > 0)
	{
		Internal_StopCollisionRaycastingTick();
		return;
	}
		
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;
	FHitResult Hit(ForceInit);
	TArray<AActor*> IgnoreActors = { GetInstigator(), GetOwner() };
	const FVector StartTrace = MeshComponentRef->GetSocketLocation(Socket_TraceStart);
	const FVector EndTrace = MeshComponentRef->GetSocketLocation(Socket_TraceEnd);
	EDrawDebugTrace::Type DebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	UKismetSystemLibrary::SphereTraceSingle(this, StartTrace, EndTrace, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoreActors, DebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f);
	if(!Hit.bBlockingHit)
	{
		return;
	}
		
	AActor* HitActor = Hit.GetActor();		
	if(HitActor && !HitActors.Contains(HitActor) && HitActor->FindComponentByClass<UEffectContainerComponent>())
	{		
		if(bCanHitMultipleEnemies && UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameState::Dead))
		{
			return;
		}
			
		if(!bRecordedHit)
		{
			bRecordedHit = true;
		}

		if(!bFriendlyFire && UCommonInteractUtils::AreActorsAllies(HitActor, GetInstigator()))
		{
			return;
		}
		
		HitActors.Add(HitActor);
		UCommonEffectUtils::ApplyEffectsToHitResult(AbilityEffects, Hit, GetInstigator());
		K2_HandleValidHit(Hit);
	}
}

void UMeleeOverlapActivation::Internal_StartAttack()
{
	HitActors.Empty();
	Internal_StartCollisionRaycastingTick();
	K2_HandleMontageActivation();
}

void UMeleeOverlapActivation::Internal_StopAttack()
{
	HitActors.Empty();
	bRecordedHit = false;
	Internal_StopCollisionRaycastingTick();
	K2_HandleMontageDeactivation();
}
