// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonEffectUtils.h"
#include "Effects/CommonEffect.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/DataTable.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "Core/ActorSystems/CommonSpawnSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/CommonInteractUtils.h"

void UCommonEffectUtils::ApplyEffectsInRadius(AActor* InstigatingActor, TArray<TSubclassOf<AActor>> EffectsToApply, FVector TraceOrigin, float TraceRadius, ETraceTypeQuery ValidationTraceType,
                                              bool bIgnoreAffiliation, bool bValidateHit, FName HitValidationBone, bool bOverrideValidationStartLocation, FVector ValidationTraceStartOverride)
{
	if(EffectsToApply.IsEmpty() || !InstigatingActor || TraceRadius < 1.f || TraceOrigin.IsZero())
	{
		return;
	}

	// Get all actors in radius around the point
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(InstigatingActor, TraceOrigin, TraceOrigin, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, {}, EDrawDebugTrace::None, HitResults, true, FLinearColor::Red, FLinearColor::Green, 1.f);

	// Only process 1 hit per actor
	TMap<AActor*, FHitResult> UniqueHitActors;
	for(FHitResult SumHit : HitResults)
	{
		AActor* HitActor = SumHit.GetActor();
		if(!HitActor || UniqueHitActors.Contains(HitActor))
		{
			continue;
		}
		UniqueHitActors.Add(HitActor, SumHit);
	}
		
	if(UniqueHitActors.Num() <= 0)
	{
		return;
	}

	// Remove any actors that are allies (unless friendly fire is allowed) or neutral 
	TArray<AActor*> HitActorArray;
	UniqueHitActors.GetKeys(HitActorArray);
	for(AActor* CurrActor : HitActorArray)
	{
		if(UCommonInteractUtils::IsActorNeutral(CurrActor))
		{
			UniqueHitActors.Remove(CurrActor);
		} else if(UCommonInteractUtils::AreActorsAllies(CurrActor, InstigatingActor) && !bIgnoreAffiliation)
		{
			UniqueHitActors.Remove(CurrActor);
		}
	}

	// If bValidateHit is true, check if there's nothing between the actor and the source location of the effect (e.g. a wall between them would invalidate the effect if bValidateHit is true) 
	if(bValidateHit)
	{
		TArray<AActor*> UniqueHitKeys;
		UniqueHitActors.GetKeys(HitActorArray);
		UniqueHitActors.GetKeys(UniqueHitKeys);
		
		for(AActor* CurrActor : UniqueHitKeys)
		{
			if(!CurrActor)
			{
				continue;
			}
			
			USkeletalMeshComponent* MeshComponent = CurrActor->FindComponentByClass<USkeletalMeshComponent>();
			if(!MeshComponent || !MeshComponent->DoesSocketExist(HitValidationBone))
			{
				continue;
			}

			// Remove this actor to check temporarily so the line trace to it isn't ignored
			HitActorArray.Remove(CurrActor);

			FHitResult ValidationLineTraceHit;
			const FVector& ValidationStart = bOverrideValidationStartLocation ? ValidationTraceStartOverride : TraceOrigin;
			UKismetSystemLibrary::LineTraceSingle(InstigatingActor, ValidationStart, MeshComponent->GetSocketLocation(HitValidationBone), ValidationTraceType, true, HitActorArray, EDrawDebugTrace::None, ValidationLineTraceHit, true, FLinearColor::Red, FLinearColor::Green, 15.f);
			
			if(ValidationLineTraceHit.bBlockingHit)
			{
				ApplyEffectsToHitResult(EffectsToApply, ValidationLineTraceHit, InstigatingActor);
			}
			
			// Re-add this actor to the line trace to it is ignored for future validations
			HitActorArray.Add(CurrActor);
		}
	} else
	{
		for(TTuple<AActor*, FHitResult> CurrHit: UniqueHitActors)
		{
			ApplyEffectsToHitResult(EffectsToApply, CurrHit.Value, InstigatingActor);
		}
	}
}

void UCommonEffectUtils::ApplyEffectAtLocation(AActor* InstigatingActor, TSubclassOf<AActor> EffectToApply, FVector Location, bool bActivateImmediately)
{
	check(InstigatingActor)
	FTransform SpawnTransform = FTransform();
	SpawnTransform.SetLocation(Location);

	check(EffectToApply)
	ACommonEffect* EffectActor = UCommonSpawnSubsystem::SpawnActorToCurrentWorld_Deferred<ACommonEffect>(InstigatingActor, EffectToApply.Get(), InstigatingActor, Cast<APawn>(InstigatingActor));
	UCommonSpawnSubsystem::FinishSpawningActor_Deferred(EffectActor, SpawnTransform);
	if(bActivateImmediately)
	{
		EffectActor->TryActivateEffect();
	}
}

void UCommonEffectUtils::ApplyEffectToActor(AActor* ReceivingActor, TSubclassOf<AActor> EffectToApply)
{
	if (!ReceivingActor || !EffectToApply)
	{
		return;
	}
	UEffectContainerComponent* EffectContainerComponent = ReceivingActor->FindComponentByClass<UEffectContainerComponent>();
	if (!EffectContainerComponent)
	{
		return;
	}
	EffectContainerComponent->TryApplyEffectToContainer(EffectToApply, ReceivingActor);
}

void UCommonEffectUtils::RemoveTaggedEffectsFromActor(AActor* ReceivingActor, const FGameplayTag& RemoveEffectsWithTag)
{
	UEffectContainerComponent* EffectContainerComponent = ReceivingActor->FindComponentByClass<UEffectContainerComponent>();
	if (!EffectContainerComponent)
	{
		return;
	}
	EffectContainerComponent->TryRemoveAllTaggedEffects(RemoveEffectsWithTag);
}

void UCommonEffectUtils::RemoveEffectsWithClassFromActor(AActor* ReceivingActor, TSubclassOf<AActor> EffectClassToRemove)
{
	UEffectContainerComponent* EffectContainerComponent = ReceivingActor->FindComponentByClass<UEffectContainerComponent>();
	if (!EffectContainerComponent)
	{
		return;
	}
	EffectContainerComponent->TryRemoveAllEffectsOfClass(EffectClassToRemove);
}

void UCommonEffectUtils::ApplyEffectsToActor(TArray<TSubclassOf<AActor>> EffectsToApply, AActor* ReceivingActor)
{
	for (const TSubclassOf<AActor> EffectClass : EffectsToApply)
	{
		ApplyEffectToActor(ReceivingActor, EffectClass);
	}
}

void UCommonEffectUtils::ApplyEffectsToHitResult(TArray<TSubclassOf<AActor>> EffectsToApply, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult)
{
	for(const TSubclassOf<AActor> EffectClass : EffectsToApply)
	{
		ApplyEffectToHitResult(EffectClass, Impact, InstigatingActor, bShouldRotateHitResult);
	}
}

void UCommonEffectUtils::ApplyEffectToHitResult(TSubclassOf<AActor> BaseEffectClass, const FHitResult& Impact, AActor* InstigatingActor, bool bShouldRotateHitResult)
{
	if (!InstigatingActor)
	{
		return;
	}

	if (!BaseEffectClass)
	{
		return;
	}
	
	if (!InstigatingActor || !BaseEffectClass)
	{
		return;
	}

	const AActor* HitActor = Impact.GetActor();
	if (!HitActor)
	{
		return;
	}
	
	UEffectContainerComponent* EffectContainerComponent = HitActor->FindComponentByClass<UEffectContainerComponent>();
	EffectContainerComponent->TryApplyEffectToContainerFromHitResult(BaseEffectClass, Impact, InstigatingActor, bShouldRotateHitResult);
}