
#include "Ability/Activation/MeleeOverlapActivation.h"

#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/LockOnComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonTagTypes.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInteractUtils.h"

void UMeleeOverlapActivation::InitActivationMechanism(UMeshComponent* OwnerMeshComponent)
{
	Super::InitActivationMechanism(OwnerMeshComponent);
	UGameplayTagComponent::RemoveTagFromActor(GetOwner(), CommonGameAbilityEvent::Active);
	HitActors.Empty();
	for(const FName& Socket : MeshComponentRef->GetAllSocketNames())
	{
		if(Socket.ToString().Contains(SocketPrefix))
		{
			Sockets.Add(Socket.ToString(), FVector::ZeroVector);
		}
	}
}

void UMeleeOverlapActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	if (!UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameAbilityEvent::Active)) {
		HitActors.Empty();
		UGameplayTagComponent::AddTagToActor(GetOwner(), CommonGameAbilityEvent::Active);
		Internal_StartCollisionRaycastingTick();
	}
	FAbilityActivationEventPayload ActivationEventPayload;
	ActivationEventPayload.bShouldStartCooldown = false;
	AbilityActivationEvent.Broadcast(ActivationEventPayload);
}

void UMeleeOverlapActivation::Deactivate()
{
	if(UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameAbilityEvent::Active))
	{
		Internal_StopAttack();
	}
	FAbilityDeactivationEventPayload DeactivationEventPayload;
	AbilityDeactivationEvent.Broadcast(DeactivationEventPayload);
}

void UMeleeOverlapActivation::Internal_StartCollisionRaycastingTick()
{
	if(!MeshComponentRef)
	{
		return;
	}
	K2_StartWeaponTrace();

	Internal_SetCurrentSocketLocations();
	GetWorld()->GetTimerManager().SetTimer(Timer_Raycasting, this, &UMeleeOverlapActivation::Internal_CheckForCollisionHit, .001f, true);
}

void UMeleeOverlapActivation::Internal_StopCollisionRaycastingTick()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer_Raycasting);
	K2_StopWeaponTrace();
	CachedComboSection = "";
}

void UMeleeOverlapActivation::Internal_CheckForCollisionHit()
{
	if(!MeshComponentRef || !UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameAbilityEvent::Active))
	{
		return;
	}
	
	TArray<FString> Keys;
	Sockets.GetKeys(Keys);
	
	for(const FString& Key : Keys)
	{
		if(!bCanHitMultipleEnemies && HitActors.Num() > 0)
		{
			break;
		}
		
		FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit(ForceInit);
		TArray<AActor*> IgnoreActors = { GetInstigator(), GetOwner() };
		const FVector StartTrace = *Sockets.Find(Key);
		const FVector EndTrace = MeshComponentRef->GetSocketLocation(FName(Key));
		EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::ForDuration;
		UKismetSystemLibrary::SphereTraceSingle(this, StartTrace, EndTrace, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoreActors, DebugTrace, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f);
		if(!Hit.bBlockingHit)
		{
			continue;
		}
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor && !HitActors.Contains(HitActor) && HitActor->FindComponentByClass<UEffectContainerComponent>())
		{
			if(bCanHitMultipleEnemies && UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameState::Dead))
			{
				break;
			}
			
			if(!bCanHitMultipleEnemies && HitActors.Num() > 0)
			{
				break;
			}
			
			if(!bRecordedHit)
			{
				bRecordedHit = true;
			}

			if(!bFriendlyFire && UCommonInteractUtils::AreActorsAllies(HitActor, GetInstigator()))
			{
				break;
			}
			
			K2_PlayHitEffects(Hit);
			HitActors.Add(HitActor);
			UCommonEffectUtils::ApplyEffectsToHitResult(AbilityEffects, Hit, GetInstigator());
		}
	}
	Internal_SetCurrentSocketLocations();
}

void UMeleeOverlapActivation::Internal_SetCurrentSocketLocations()
{
	TArray<FString> Keys;
	Sockets.GetKeys(Keys);
	for(const FString& Key : Keys)
	{
		Sockets.Add(Key, MeshComponentRef->GetSocketLocation(FName(Key)));	
	}	
}

void UMeleeOverlapActivation::Internal_StartAttack()
{
	const APawn* CurrInstigator = GetInstigator();
	if(!CurrInstigator)
	{
		return;
	}
	
	if(ULockOnComponent* LockOnComponent = GetInstigator()->FindComponentByClass<ULockOnComponent>(); LockOnComponent && CurrInstigator->IsPlayerControlled())
	{
		LockOnComponent->InterpToBestTargetForMeleeAttack();
	}
	K2_WeaponActivated();
}

void UMeleeOverlapActivation::Internal_StopAttack()
{
	UGameplayTagComponent::RemoveTagFromActor(GetOwner(), CommonGameAbilityEvent::Active);
	HitActors.Empty();
	bRecordedHit = false;
	Internal_StopCollisionRaycastingTick();
}
