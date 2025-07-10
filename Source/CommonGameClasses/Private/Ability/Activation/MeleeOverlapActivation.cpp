
#include "Ability/Activation/MeleeOverlapActivation.h"

#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonTagTypes.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInteractUtils.h"

void UMeleeOverlapActivation::PostInitProperties()
{
	Super::PostInitProperties();
	if (TraceForObjectTypes.Num() == 0)
	{
		TraceForObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	}
}

void UMeleeOverlapActivation::InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent)
{
	Super::InitActivationMechanism(OwnerMeshComponent);
	HitActors.Empty();
	check(MeshComponentRef.IsValid())
	check(MeshComponentRef->DoesSocketExist(Socket_TraceStart));
	check(MeshComponentRef->DoesSocketExist(Socket_TraceEnd));
}

void UMeleeOverlapActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	Super::Activate(TriggerEventPayload);
	Internal_StartAttack();
	FAbilityActivationEventPayload ActivationEventPayload;
	ActivationEventPayload.bShouldStartCooldown = false;
	AbilityActivationEvent.Broadcast(ActivationEventPayload);
	BPI_OnActivation();
}

void UMeleeOverlapActivation::Deactivate()
{
	Super::Deactivate();
	Internal_StopAttack();
	FAbilityDeactivationEventPayload DeactivationEventPayload;
	AbilityDeactivationEvent.Broadcast(DeactivationEventPayload);
	BPI_OnDeactivation();
}

void UMeleeOverlapActivation::Internal_StartCollisionRaycastingTick()
{
	if(!MeshComponentRef.IsValid())
	{
		return;
	}
	BPI_HandleWeaponTraceStart();
	GetWorld()->GetTimerManager().SetTimer(Timer_Raycasting, this, &ThisClass::Internal_CheckForCollisionHit, TraceTickRate, true);
}

void UMeleeOverlapActivation::Internal_StopCollisionRaycastingTick()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer_Raycasting);
	BPI_HandleWeaponTraceEnd();
}

void UMeleeOverlapActivation::Internal_CheckForCollisionHit()
{
	if(!MeshComponentRef.IsValid() || !UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonAbilityStateTags::Activated))
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
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, StartTrace, EndTrace, TraceRadius, TraceForObjectTypes, false, IgnoreActors, DebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f);
	if(!Hit.bBlockingHit)
	{
		return;
	}
		
	AActor* HitActor = Hit.GetActor();		
	if(HitActor && !HitActors.Contains(HitActor) && HitActor->FindComponentByClass<UEffectContainerComponent>())
	{		
		if(UGameplayTagComponent::ActorHasGameplayTag(HitActor, CommonStateTags::Dead))
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
		BPI_HandleValidHit(Hit);
	}
}

void UMeleeOverlapActivation::Internal_StartAttack()
{
	HitActors.Empty();
	Internal_StartCollisionRaycastingTick();
	BPI_HandleMontageActivation();
}

void UMeleeOverlapActivation::Internal_StopAttack()
{
	HitActors.Empty();
	bRecordedHit = false;
	Internal_StopCollisionRaycastingTick();
	BPI_HandleMontageDeactivation();
}
