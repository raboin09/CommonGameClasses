
#include "Ability/Activation/MeleeOverlapActivation.h"

#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/LockOnComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCoreTypes.h"
#include "Types/CommonTagTypes.h"
#include "Utils/CommonEffectUtils.h"
#include "Utils/CommonInteractUtils.h"

AMeleeOverlapActivation::AMeleeOverlapActivation()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMeleeOverlapActivation::BeginPlay()
{
	Super::BeginPlay();
	UGameplayTagComponent::RemoveTagFromActor(GetOwner(), TAG_STATE_ACTIVE);
	HitActors.Empty();
	for(const FName& Socket : MeshComponentRef->GetAllSocketNames())
	{
		if(Socket.ToString().Contains(SocketPrefix))
		{
			Sockets.Add(Socket.ToString(), FVector::ZeroVector);
		}
	}
}

void AMeleeOverlapActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	if (!UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), TAG_STATE_ACTIVE)) {
		HitActors.Empty();
		UGameplayTagComponent::AddTagToActor(GetOwner(), TAG_STATE_ACTIVE);
		Internal_StartCollisionRaycastingTick();
	}
	FAbilityActivationEventPayload ActivationEventPayload;
	ActivationEventPayload.bShouldStartCooldown = false;
	AbilityActivationEvent.Broadcast(ActivationEventPayload);
}

void AMeleeOverlapActivation::Deactivate()
{
	COMMON_PRINTSCREEN("Deactivating")
	if(UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), TAG_STATE_ACTIVE))
	{
		Internal_StopAttack();
	}
	FAbilityDeactivationEventPayload DeactivationEventPayload;
	AbilityDeactivationEvent.Broadcast(DeactivationEventPayload);
	UGameplayTagComponent::RemoveTagFromActor(GetOwner(), TAG_ABILITY_COMMITTED);
}

void AMeleeOverlapActivation::Internal_StartCollisionRaycastingTick()
{
	if(!MeshComponentRef)
	{
		return;
	}
	K2_StartWeaponTrace();

	Internal_SetCurrentSocketLocations();
	GetWorldTimerManager().SetTimer(Timer_Raycasting, this, &AMeleeOverlapActivation::Internal_CheckForCollisionHit, .001f, true);
}

void AMeleeOverlapActivation::Internal_StopCollisionRaycastingTick()
{
	GetWorldTimerManager().ClearTimer(Timer_Raycasting);
	K2_StopWeaponTrace();
	CachedComboSection = "";
}

void AMeleeOverlapActivation::Internal_CheckForCollisionHit()
{
	if(!MeshComponentRef || !UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), TAG_STATE_ACTIVE))
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
		TArray<AActor*> IgnoreActors = { GetInstigator(), this, GetOwner() };
		const FVector StartTrace = *Sockets.Find(Key);
		const FVector EndTrace = MeshComponentRef->GetSocketLocation(FName(Key));
		UKismetSystemLibrary::SphereTraceSingle(this, StartTrace, EndTrace, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoreActors, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f);
		if(!Hit.bBlockingHit)
		{
			continue;
		}
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor && !HitActors.Contains(HitActor) && HitActor->FindComponentByClass<UEffectContainerComponent>())
		{
			if(bCanHitMultipleEnemies && UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), TAG_STATE_DEAD))
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

void AMeleeOverlapActivation::Internal_SetCurrentSocketLocations()
{
	TArray<FString> Keys;
	Sockets.GetKeys(Keys);
	for(const FString& Key : Keys)
	{
		Sockets.Add(Key, MeshComponentRef->GetSocketLocation(FName(Key)));	
	}	
}

void AMeleeOverlapActivation::Internal_StartAttack()
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

void AMeleeOverlapActivation::Internal_StopAttack()
{
	UGameplayTagComponent::RemoveTagFromActor(GetOwner(), TAG_STATE_ACTIVE);
	HitActors.Empty();
	bRecordedHit = false;
	Internal_StopCollisionRaycastingTick();
}
