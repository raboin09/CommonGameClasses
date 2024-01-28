#include "Effects/CommonEffect.h"

#include "ActorComponent/GameplayTagComponent.h"
#include "Utils/CommonCombatUtils.h"


ACommonEffect::ACommonEffect()
{
 	PrimaryActorTick.bCanEverTick = false;
	SetAutoDestroyWhenFinished(false);
	EffectData = CreateDefaultSubobject<UEffectData>(TEXT("EffectData"));
}

void ACommonEffect::SetEffectContext(const FEffectContext& InContext)
{
	EffectContext = InContext;
}

void ACommonEffect::Internal_AddAndRemoveTagsFromReceiver_Activation()
{
	UGameplayTagComponent::AddTagsToActor(EffectContext.ReceivingActor.Get(), GetEffectInitializationData().TagsToApply);
	UGameplayTagComponent::RemoveTagsFromActor(EffectContext.ReceivingActor.Get(), GetEffectInitializationData().TagsToRemove);
}

void ACommonEffect::Internal_AddAndRemoveTagsFromReceiver_Deactivation()
{
	if(!EffectData || !EffectData->InitializationData.bShouldReverseChangesAfterDestroy)
	{
		return;
	}
	UGameplayTagComponent::RemoveTagsFromActor(EffectContext.ReceivingActor.Get(), GetEffectInitializationData().TagsToApply);
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
	K2_PlayEffectSound();
	K2_PlayEffectVFX(EffectData->bAttachVFXToActor);
}

bool ACommonEffect::CanActivateEffect()
{
	if (UGameplayTagComponent::ActorHasAnyGameplayTags(EffectContext.ReceivingActor.Get(), GetBlockedTags()))
	{
		return false;
	}

	if (!UGameplayTagComponent::ActorHasAllGameplayTags(EffectContext.ReceivingActor.Get(), GetRequiredTags()))
	{
		return false;
	}

	if(EffectData && EffectData->Conditions)
	{
		return EffectData->Conditions->AreConditionsTrue(EffectContext);
	}

	if(!K2_CanActivateEffect())
	{
		return false;
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
	K2_ActivateEffect();
	return true;
}

void ACommonEffect::DestroyEffect()
{
	Internal_AddAndRemoveTagsFromReceiver_Deactivation();
	K2_DestroyEffect();
	SetLifeSpan(1.f);
}