﻿#include "Actors/CommonEffect.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/GameplayTagComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Utils/CombatUtils.h"
#include "Sound/SoundCue.h"
#include "Utils/EffectUtils.h"


ACommonEffect::ACommonEffect()
{
 	PrimaryActorTick.bCanEverTick = false;
	SetAutoDestroyWhenFinished(false);
	// if(!EffectDataObj)
	// {
	// 	EffectDataObj = Cast<UBaseEffectData>(UBaseEffectData::StaticClass()->GetDefaultObject());
	// }
}

void ACommonEffect::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	K2_OnDestroyEffect();
	Super::EndPlay(EndPlayReason);
}

void ACommonEffect::K2_ActivateEffect_Implementation()
{
	check(false)
}

void ACommonEffect::K2_OnDestroyEffect_Implementation()
{
	if(EffectVFX)
	{
		EffectVFX->Deactivate();
	}
}

void ACommonEffect::Internal_PlayEffectSound()
{
	if(K2_GetEffectSound())
	{
		UGameplayStatics::SpawnSoundAtLocation(EffectContext.InstigatingActor, K2_GetEffectSound(), GetActorLocation());
	}
}

void ACommonEffect::Internal_PlayEffectParticleSystem()
{
	const bool bReceivingActorIsPawn = EffectContext.ReceivingActor ? EffectContext.ReceivingActor->IsA(APawn::StaticClass()) : false;
	const bool bShouldAttachVFX = EffectDataObj ? EffectDataObj->bAttachVFXToActor : false; 
	if(!bShouldAttachVFX || !bReceivingActorIsPawn)
	{
		if(UNiagaraSystem* CastedNiagaraSystem = Cast<UNiagaraSystem>(K2_GetEffectParticleSystem()))
		{
			EffectVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(EffectContext.InstigatingActor, CastedNiagaraSystem, GetActorLocation(), GetActorRotation());
		}
		else if(UParticleSystem* CastedParticleSystem = Cast<UParticleSystem>(K2_GetEffectParticleSystem()))
		{
			EffectVFX = UGameplayStatics::SpawnEmitterAtLocation(EffectContext.InstigatingActor, CastedParticleSystem, GetActorLocation(), GetActorRotation());
		}
	} else
	{
		if(UMeshComponent* ActorMesh = EffectContext.ReceivingActor->FindComponentByClass<USkeletalMeshComponent>())
		{
			if(UNiagaraSystem* CastedNiagaraSystem = Cast<UNiagaraSystem>(K2_GetEffectParticleSystem()))
			{
				EffectVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(CastedNiagaraSystem, ActorMesh, "spine_03", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, true);
			}
			else if(UParticleSystem* CastedParticleSystem = Cast<UParticleSystem>(K2_GetEffectParticleSystem()))
			{
				EffectVFX = UGameplayStatics::SpawnEmitterAttached(CastedParticleSystem, ActorMesh, "spine_03", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, true);
			}
		}
	}
}

void ACommonEffect::Internal_AddAndRemoveTagsFromReceiver_Activation()
{
	UGameplayTagComponent::AddTagsToActor(EffectContext.ReceivingActor, GetEffectInitializationData().TagsToApply);
	UGameplayTagComponent::RemoveTagsFromActor(EffectContext.ReceivingActor, GetEffectInitializationData().TagsToRemove);
}

void ACommonEffect::Internal_AddAndRemoveTagsFromReceiver_Deactivation()
{
	if(!EffectDataObj || !EffectDataObj->EffectData.bShouldReverseEffectsOnDestroy)
	{
		return;
	}
	UGameplayTagComponent::RemoveTagsFromActor(EffectContext.ReceivingActor, GetEffectInitializationData().TagsToApply);
}

bool ACommonEffect::Internal_IsValidHeadshot() const
{
	bool bPlayerControlled = false;	
	if(const APawn* CastedPawn = Cast<APawn>(EffectContext.SurfaceHit.GetActor()))
	{
		bPlayerControlled = CastedPawn->IsPlayerControlled();	
	}
	return UCombatUtils::IsBoneNameHead(EffectContext.SurfaceHit.BoneName) && !bPlayerControlled;
}

void ACommonEffect::PlayEffectFX()
{
	Internal_PlayEffectSound();
	Internal_PlayEffectParticleSystem();
}

void ACommonEffect::ActivateEffect()
{
	PlayEffectFX();
	Internal_AddAndRemoveTagsFromReceiver_Activation();
	K2_ActivateEffect();
}

void ACommonEffect::DestroyEffect()
{
	Internal_AddAndRemoveTagsFromReceiver_Deactivation();
	if(IsValid(this))
	{
		SetLifeSpan(1.f);
	}
}

UFXSystemAsset* ACommonEffect::K2_GetEffectParticleSystem_Implementation()
{
	if(!EffectDataObj || EffectDataObj->ImpactVFXRowHandle.IsNull())
	{
		return nullptr;
	}
	const UPhysicalMaterial* PhysicalMaterial = EffectContext.SurfaceHit.PhysMaterial.Get();
	return UEffectUtils::GetVFXAssetFromKey(EffectDataObj->ImpactVFXRowHandle, PhysicalMaterial, Internal_IsValidHeadshot());
}

USoundCue* ACommonEffect::K2_GetEffectSound_Implementation()
{
	if(!EffectDataObj || EffectDataObj->ImpactSFXRowHandle.IsNull())
	{
		return nullptr;
	}
	const UPhysicalMaterial* PhysicalMaterial = EffectContext.SurfaceHit.PhysMaterial.Get();
	return UEffectUtils::GetSFXAssetFromKey(EffectDataObj->ImpactSFXRowHandle, PhysicalMaterial, Internal_IsValidHeadshot());
}