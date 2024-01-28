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

void UCommonEffectUtils::ApplyEffectsToHitResultsInRadius(AActor* InstigatingActor, TArray<TSubclassOf<AActor>> EffectsToApply, FVector TraceLocation, float TraceRadius, ETraceTypeQuery ValidationTraceType, EAffiliation AffectedAffiliation, bool bValidateHit, FVector ValidationTraceStartLocation, FName HitValidationBone)
{
	if(EffectsToApply.IsEmpty() || !InstigatingActor || TraceRadius < 1.f || TraceLocation.IsZero())
	{
		return;
	}

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(InstigatingActor, TraceLocation, TraceLocation, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, {}, EDrawDebugTrace::None, HitResults, true, FLinearColor::Red, FLinearColor::Green, 1.f);

	TArray<AActor*> AllHitActors;
	for(FHitResult SumHit : HitResults)
	{
		AllHitActors.AddUnique(SumHit.GetActor());
	}

	if(HitResults.Num() <= 0)
	{
		return;
	}

	TArray<AActor*> CulledHitActors = AllHitActors;
	for(AActor* CurrActor : AllHitActors)
	{
		switch (AffectedAffiliation) {
		case EAffiliation::Allies:
				if(!UCommonInteractUtils::AreActorsAllies(CurrActor, InstigatingActor))
				{
					CulledHitActors.Remove(CurrActor);
				}
				break;
			case EAffiliation::Enemies:
				if(!UCommonInteractUtils::AreActorsEnemies(CurrActor, InstigatingActor))
				{
					CulledHitActors.Remove(CurrActor);
				}
				break;
			case EAffiliation::Neutral:
				if(!UCommonInteractUtils::IsActorNeutral(CurrActor))
				{
					CulledHitActors.Remove(CurrActor);
				}
				break;
			case EAffiliation::All:
			default:
				break;
		}
	}
	
	if(bValidateHit)
	{
		TArray<AActor*> ValidatedHitActors;
		for(AActor* CurrActor : CulledHitActors)
		{
			if(!CurrActor)
			{
				continue;
			}
		
			if(ValidatedHitActors.Contains(CurrActor))
			{
				continue;
			}

			USkeletalMeshComponent* MeshComponent = CurrActor->FindComponentByClass<USkeletalMeshComponent>();
			if(!MeshComponent || !MeshComponent->DoesSocketExist(HitValidationBone))
			{
				continue;
			}

			CulledHitActors.Remove(CurrActor);

			FHitResult ValidationLineTraceHit;
			UKismetSystemLibrary::LineTraceSingle(InstigatingActor, ValidationTraceStartLocation, MeshComponent->GetSocketLocation(HitValidationBone), ValidationTraceType, true, CulledHitActors, EDrawDebugTrace::None, ValidationLineTraceHit, true, FLinearColor::Red, FLinearColor::Green, 15.f);
			if(ValidationLineTraceHit.bBlockingHit)
			{
				ApplyEffectsToHitResult(EffectsToApply, ValidationLineTraceHit, InstigatingActor);
			}
			
			ValidatedHitActors.AddUnique(CurrActor);
		}
	} else
	{
		for(FHitResult Hit : HitResults)
		{
			ApplyEffectsToHitResult(EffectsToApply, Hit, InstigatingActor);
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
