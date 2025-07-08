#include "Effects/CommonEffect.h"
#include "ActorComponent/GameplayTagComponent.h"


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

void ACommonEffect::PlayEffectFX()
{
	BPI_PlayEffectSound(EffectContext);
	BPI_PlayEffectVFX(EffectContext);
}

bool ACommonEffect::CanActivateEffect()
{
	if (GetBlockedTags().Num() != 0 && UGameplayTagComponent::ActorHasAnyGameplayTags(EffectContext.ReceivingActor.Get(), GetBlockedTags()))
	{
		return false;
	}

	if (GetRequiredTags().Num() != 0 && !UGameplayTagComponent::ActorHasAllGameplayTags(EffectContext.ReceivingActor.Get(), GetRequiredTags()))
	{
		return false;
	}

	if(EffectData && EffectData->Conditions)
	{
		return EffectData->Conditions->AreConditionsTrue(EffectContext);
	}

	if(!BPI_CanActivateEffect())
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
	BPI_ActivateEffect();
	return true;
}

void ACommonEffect::DestroyEffect()
{
	Internal_AddAndRemoveTagsFromReceiver_Deactivation();
	BPI_DestroyEffect();
	SetLifeSpan(1.f);
}