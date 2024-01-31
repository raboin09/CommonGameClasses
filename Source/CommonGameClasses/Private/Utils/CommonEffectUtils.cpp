// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonEffectUtils.h"
#include "Effects/CommonEffect.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/DataTable.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/EffectContainerComponent.h"
#include "ActorComponent/ShieldEnergyComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CommonCombatTypes.h"
#include "Utils/CommonInteractUtils.h"
#include "Utils/CommonWorldUtils.h"

void UCommonEffectUtils::ApplyEffectsInRadius(AActor* InstigatingActor, TArray<TSubclassOf<AActor>> EffectsToApply, FVector TraceLocation, float TraceRadius, ETraceTypeQuery ValidationTraceType, bool bIgnoreAffiliation, bool bValidateHit, FVector ValidationTraceStartLocation, FName HitValidationBone)
{
	if(EffectsToApply.IsEmpty() || !InstigatingActor || TraceRadius < 1.f || TraceLocation.IsZero())
	{
		return;
	}

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(InstigatingActor, TraceLocation, TraceLocation, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, {}, EDrawDebugTrace::None, HitResults, true, FLinearColor::Red, FLinearColor::Green, 1.f);

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

	TArray<AActor*> HitActorArray;
	UniqueHitActors.GetKeys(HitActorArray);
	for(AActor* CurrActor : HitActorArray)
	{
		if(!CurrActor)
		{
			UniqueHitActors.Remove(CurrActor);
			continue;
		}
		
		if(!UniqueHitActors.Contains(CurrActor))
		{
			continue;
		}
		
		if(!bIgnoreAffiliation && UCommonInteractUtils::AreActorsAllies(CurrActor, InstigatingActor))
		{
			UniqueHitActors.Remove(CurrActor);
		}
	}
	
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
			UKismetSystemLibrary::LineTraceSingle(InstigatingActor, ValidationTraceStartLocation, MeshComponent->GetSocketLocation(HitValidationBone), ValidationTraceType, true, HitActorArray, EDrawDebugTrace::None, ValidationLineTraceHit, true, FLinearColor::Red, FLinearColor::Green, 15.f);
			
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
	FTransform SpawnTransform = FTransform();
	SpawnTransform.SetLocation(Location);

	check(EffectToApply)
	ACommonEffect* EffectActor = UCommonWorldUtils::SpawnActorToCurrentWorld_Deferred<ACommonEffect>(EffectToApply.Get(), InstigatingActor, Cast<APawn>(InstigatingActor));
	UCommonWorldUtils::FinishSpawningActor_Deferred(EffectActor, SpawnTransform);
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

void UCommonEffectUtils::TryAddMaxWoundsToActor(const AActor* ReceivingActor, float MaxWoundsToAdd)
{
	if(!ReceivingActor)
		return;
	UHealthComponent* HealthComponent = ReceivingActor->FindComponentByClass<UHealthComponent>();
	if (!HealthComponent)
	{
		return;
	}
	HealthComponent->AddMaxWounds(MaxWoundsToAdd);
}

void UCommonEffectUtils::TryApplyDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent)
{
	if(!ReceivingActor)
		return;

	if(UShieldEnergyComponent* ShieldEnergyComponent = ReceivingActor->FindComponentByClass<UShieldEnergyComponent>())
	{
		Damage = ShieldEnergyComponent->TakeShieldDamage(Damage, InstigatingActor, HitReactEvent);
	}

	if(UHealthComponent* HealthComponent = ReceivingActor->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->TakeDamage(Damage, InstigatingActor, HitReactEvent);
	}
}

void UCommonEffectUtils::TryApplyHealthDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent)
{
	if(!ReceivingActor)
		return;

	if(UHealthComponent* HealthComponent = ReceivingActor->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->TakeDamage(Damage, InstigatingActor, HitReactEvent);
	}

}

void UCommonEffectUtils::TryApplyHealthHealToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Heal)
{
	if(!ReceivingActor)
		return;

	if(UHealthComponent* HealthComponent = ReceivingActor->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->ApplyHeal(Heal, InstigatingActor);
	}
}

void UCommonEffectUtils::TryApplyShieldDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent)
{
	if(!ReceivingActor)
		return;
	
	if(UShieldEnergyComponent* ShieldEnergyComponent = ReceivingActor->FindComponentByClass<UShieldEnergyComponent>())
	{
		ShieldEnergyComponent->TakeShieldDamage(Damage, InstigatingActor, HitReactEvent);
	}
}

void UCommonEffectUtils::TryApplyShieldHealToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Heal)
{
	if(!ReceivingActor)
		return;
	
	if(UShieldEnergyComponent* ShieldEnergyComponent = ReceivingActor->FindComponentByClass<UShieldEnergyComponent>())
	{
		ShieldEnergyComponent->ApplyShieldHeal(Heal);
	}
}
