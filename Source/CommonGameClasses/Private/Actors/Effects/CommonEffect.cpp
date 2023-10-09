#include "Actors/Effects/CommonEffect.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Utils/CommonCombatUtils.h"
#include "Sound/SoundCue.h"
#include "Utils/CommonEffectUtils.h"


ACommonEffect::ACommonEffect()
{
 	PrimaryActorTick.bCanEverTick = false;
	SetAutoDestroyWhenFinished(false);
}

void ACommonEffect::BeginPlay()
{
	Super::BeginPlay();
}

void ACommonEffect::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	K2_OnDestroyEffect();
	Super::EndPlay(EndPlayReason);
}

void ACommonEffect::SetEffectContext(const FEffectContext& InContext)
{
	EffectContext = InContext;
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
	const bool bShouldAttachVFX = EffectData ? EffectData->bAttachVFXToActor : false; 
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
	if(!EffectData || !EffectData->InitializationData.bShouldReverseChangesAfterDestroy)
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
	return UCommonCombatUtils::IsBoneNameHead(EffectContext.SurfaceHit.BoneName) && !bPlayerControlled;
}

void ACommonEffect::PlayEffectFX()
{
	Internal_PlayEffectSound();
	Internal_PlayEffectParticleSystem();
}

bool ACommonEffect::CanActivateEffect()
{
	if (UGameplayTagComponent::ActorHasAnyGameplayTags(EffectContext.ReceivingActor, GetBlockedTags()))
	{
		return false;
	}

	if (!UGameplayTagComponent::ActorHasAllGameplayTags(EffectContext.ReceivingActor, GetRequiredTags()))
	{
		return false;
	}

	if(EffectData && EffectData->Conditions)
	{
		return EffectData->Conditions->AreConditionsTrue(EffectContext);
	}
	return true;
}

bool ACommonEffect::TryActivateEffect()
{
	if(!CanActivateEffect())
	{
		return false;
	}
	PlayEffectFX();
	Internal_AddAndRemoveTagsFromReceiver_Activation();
	K2_OnActivateEffect();
	return true;
}

void ACommonEffect::DestroyEffect()
{
	Internal_AddAndRemoveTagsFromReceiver_Deactivation();
	if(EffectVFX)
	{
		EffectVFX->Deactivate();
	}
	K2_OnDestroyEffect();
	SetLifeSpan(1.f);
}

UFXSystemAsset* ACommonEffect::K2_GetEffectParticleSystem_Implementation()
{
	if(!EffectData || EffectData->ImpactVFXRowHandle.IsNull())
	{
		return nullptr;
	}
	const UPhysicalMaterial* PhysicalMaterial = EffectContext.SurfaceHit.PhysMaterial.Get();
	return UCommonEffectUtils::GetVFXAssetFromKey(EffectData->ImpactVFXRowHandle, PhysicalMaterial, Internal_IsValidHeadshot());
}

USoundCue* ACommonEffect::K2_GetEffectSound_Implementation()
{
	if(!EffectData || EffectData->ImpactSFXRowHandle.IsNull())
	{
		return nullptr;
	}
	const UPhysicalMaterial* PhysicalMaterial = EffectContext.SurfaceHit.PhysMaterial.Get();
	return UCommonEffectUtils::GetSFXAssetFromKey(EffectData->ImpactSFXRowHandle, PhysicalMaterial, Internal_IsValidHeadshot());
}