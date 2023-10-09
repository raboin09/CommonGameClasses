// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonEffectUtils.h"
#include "Actors/Effects/CommonEffect.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/DataTable.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/EffectContainerComponent.h"
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

	ACommonEffect* EffectActor = UCommonWorldUtils::SpawnActorToCurrentStreamedWorld_Deferred<ACommonEffect>(EffectToApply, InstigatingActor, Cast<APawn>(InstigatingActor));
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

void UCommonEffectUtils::TryApplyHealToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Heal)
{
	if(!ReceivingActor)
		return;
	UHealthComponent* HealthComponent = ReceivingActor->FindComponentByClass<UHealthComponent>();
	
	if (!HealthComponent)
		return;
	HealthComponent->ApplyHeal(Heal, InstigatingActor);
}

void UCommonEffectUtils::TryApplyDamageToActor(const AActor* ReceivingActor, AActor* InstigatingActor, float Damage, const FDamageHitReactEvent& HitReactEvent)
{
	if(!ReceivingActor)
		return;
	UHealthComponent* HealthComponent = ReceivingActor->FindComponentByClass<UHealthComponent>();
	
	if (!HealthComponent)
		return;
	HealthComponent->TakeDamage(Damage, InstigatingActor, HitReactEvent);
}

UFXSystemAsset* UCommonEffectUtils::GetVFXAssetFromKey(const FDataTableRowHandle& RowHandle, const UPhysicalMaterial* SurfaceMaterial, bool bIsValidHeadshot)
{
	if(RowHandle.IsNull() || RowHandle.RowName.IsNone())
	{
		return nullptr;
	}
	
	const FEffectImpactVFX* FoundRow = RowHandle.DataTable->FindRow<FEffectImpactVFX>(RowHandle.RowName, RowHandle.RowName.ToString());
	if(!FoundRow)
	{
		return nullptr;
	}	
	const FEffectImpactVFX& OutRow = *FoundRow;
	
	if (bIsValidHeadshot && OutRow.FleshHeadshotFX)
	{
		return OutRow.FleshHeadshotFX;
	}

	if(!IsValid(SurfaceMaterial))
	{
		return OutRow.DefaultFX;  
	}

	UNiagaraSystem* SelectedParticle;
	switch (SurfaceMaterial->SurfaceType)
	{
	case COMMON_SURFACE_Default: SelectedParticle = OutRow.DefaultFX;
		break;
	case COMMON_SURFACE_Concrete: SelectedParticle = OutRow.ConcreteFX;
		break;
	case COMMON_SURFACE_Dirt: SelectedParticle = OutRow.DirtFX;
		break;
	case COMMON_SURFACE_Water: SelectedParticle = OutRow.WaterFX;
		break;
	case COMMON_SURFACE_Metal: SelectedParticle = OutRow.MetalFX;
		break;
	case COMMON_SURFACE_Wood: SelectedParticle = OutRow.WoodFX;
		break;
	case COMMON_SURFACE_Grass: SelectedParticle = OutRow.GrassFX;
		break;
	case COMMON_SURFACE_Glass: SelectedParticle = OutRow.GlassFX;
		break;
	case COMMON_SURFACE_Flesh: SelectedParticle = OutRow.FleshFX;
		break;
	case COMMON_SURFACE_Plastic: SelectedParticle = OutRow.PlasticFX;
		break;
	case COMMON_SURFACE_Sand: SelectedParticle = OutRow.SandFX;
		break;
	case COMMON_SURFACE_Ice: SelectedParticle = OutRow.FleshFX;
		break;
	default: SelectedParticle = OutRow.DefaultFX;
	}
	return SelectedParticle ? SelectedParticle : OutRow.DefaultFX;
}

USoundCue* UCommonEffectUtils::GetSFXAssetFromKey(const FDataTableRowHandle& RowHandle, const UPhysicalMaterial* SurfaceMaterial, bool bIsValidHeadshot)
{
	if(RowHandle.IsNull() || RowHandle.RowName.IsNone())
	{
		return nullptr;
	}
	
	const FEffectImpactSFX* FoundRow = RowHandle.DataTable->FindRow<FEffectImpactSFX>(RowHandle.RowName, SurfaceMaterial->GetFullName());
	if(!FoundRow)
	{
		return nullptr;
	}

	if(!IsValid(SurfaceMaterial))
	{
		return FoundRow->DefaultSound;
	}
	
	const FEffectImpactSFX& OutRow = *FoundRow;	
	if (bIsValidHeadshot && OutRow.FleshHeadshotSound)
	{
		return OutRow.FleshHeadshotSound;
	}

	USoundCue* SelectedSound;
	switch (SurfaceMaterial->SurfaceType)
	{
	case COMMON_SURFACE_Concrete: SelectedSound = OutRow.ConcreteSound;
		break;
	case COMMON_SURFACE_Dirt: SelectedSound = OutRow.DirtSound;
		break;
	case COMMON_SURFACE_Water: SelectedSound = OutRow.WaterSound;
		break;
	case COMMON_SURFACE_Metal: SelectedSound = OutRow.MetalSound;
		break;
	case COMMON_SURFACE_Wood: SelectedSound = OutRow.WoodSound;
		break;
	case COMMON_SURFACE_Grass: SelectedSound = OutRow.GrassSound;
		break;
	case COMMON_SURFACE_Glass: SelectedSound = OutRow.GlassSound;
		break;
	case COMMON_SURFACE_Flesh: SelectedSound = OutRow.FleshSound;
		break;
	case COMMON_SURFACE_Plastic: SelectedSound = OutRow.PlasticSound;
		break;
	case COMMON_SURFACE_Sand: SelectedSound = OutRow.SandSound;
		break;
	case COMMON_SURFACE_Ice: SelectedSound = OutRow.IceSound;
		break;
	default: SelectedSound = OutRow.DefaultSound;
	}
	return SelectedSound ? SelectedSound : OutRow.DefaultSound;
}

