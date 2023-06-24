#include "Actors/Effects/CommonStatModifierEffect.h"

#include "ActorComponent/GameplayTagComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/CommonCombatUtils.h"
#include "Utils/CommonEffectUtils.h"

void ACommonStatModifierEffect::ActivateEffect()
{
	K2_ApplyStatChange(CalculateModifierValues());
	Super::ActivateEffect();
}

void ACommonStatModifierEffect::DestroyEffect()
{
	Super::DestroyEffect();
	if(EffectDataObj->EffectData.bShouldReverseEffectsOnDestroy)
	{
		ReversalFunc();
	}
}

void ACommonStatModifierEffect::K2_ApplyStatChange_Implementation(float ModifiedStatValue)
{
	switch (StatEffectDataObj->StatToModify) {
	case EEffectStatType::Health_MaxWounds:
		Internal_HealthMaxWounds(ModifiedStatValue);
		break;
	case EEffectStatType::Health_Heal:
		Internal_HealthHeal(ModifiedStatValue);
		break;
	case EEffectStatType::Health_Damage:
		Internal_HealthDamage(ModifiedStatValue);
		break;
	case EEffectStatType::Movespeed:
		Internal_MovespeedStatChange(ModifiedStatValue);
		break;
	default:;
	}
}

float ACommonStatModifierEffect::CalculateHeadshotDamage(float ModifiedStatValue) const
{
	if(StatEffectDataObj->bAddDamageForHeadshots && UCommonCombatUtils::IsBoneNameHead(EffectContext.SurfaceHit.BoneName))
	{
		return ModifiedStatValue * StatEffectDataObj->HeadshotModifier;
	}
	return ModifiedStatValue;
}

float ACommonStatModifierEffect::CalculateModifierValues() const
{
	TArray<FGameplayTag> ModifierKeys;
	EffectDataObj->EffectModifiers.GetKeys(ModifierKeys);
	float ModifiedValue = StatEffectDataObj->BaseModifierValue;
	for(const FGameplayTag& CurrentTag : ModifierKeys)
	{
		CalculateModifier(*EffectDataObj->EffectModifiers.Find(CurrentTag), CurrentTag, ModifiedValue);
	}
	return ModifiedValue;
}

void ACommonStatModifierEffect::CalculateModifier(const FModifierExpression& ModifierExpression, const FGameplayTag& ModifierTag, float& ModifiedBaseValue) const
{
	AActor* ModifierActor = nullptr;
	switch (ModifierExpression.ModifierActor)
	{
	case EModifierActor::Instigator:
		ModifierActor = EffectContext.InstigatingActor;
		break;
	case EModifierActor::Receiver:
		ModifierActor = EffectContext.ReceivingActor;
		break;
	default:
		break;
	}

	if(!UGameplayTagComponent::ActorHasGameplayTag(ModifierActor, ModifierTag))
	{
		return;
	}
	
	switch (ModifierExpression.Operation)
	{
	case EModifierOperator::Add:
		ModifiedBaseValue += ModifierExpression.ModifierOperand;
		break;
	case EModifierOperator::Subtract:
		ModifiedBaseValue -= ModifierExpression.ModifierOperand;
		break;
	case EModifierOperator::Multiply:
		ModifiedBaseValue *= ModifierExpression.ModifierOperand;
		break;
	default: ;
	}
}

void ACommonStatModifierEffect::Internal_MovespeedStatChange(float ModifiedStatValue)
{
	if(UCharacterMovementComponent* MovementComponent = EffectContext.ReceivingActor->FindComponentByClass<UCharacterMovementComponent>())
	{
		MovementComponent->MaxWalkSpeed += ModifiedStatValue;
		ReversalFunc = [ModifiedStatValue, MovementComponent]
		{
			MovementComponent->MaxWalkSpeed -= ModifiedStatValue;
		};
	}
}

void ACommonStatModifierEffect::Internal_HealthDamage(float ModifiedStatValue) const
{
	FDamageHitReactEvent HitReactEvent;
	HitReactEvent.DamageTaken = CalculateHeadshotDamage(ModifiedStatValue);
	const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(EffectContext.SurfaceHit.TraceStart, EffectContext.ReceivingActor->GetActorLocation());
	HitReactEvent.HitDirection = LookAt.Vector().GetSafeNormal();
	HitReactEvent.HitResult = EffectContext.SurfaceHit;
	HitReactEvent.DeathReactType = StatEffectDataObj->DeathImpulse;
	HitReactEvent.HitReactType = StatEffectDataObj->HitImpulse;
	UCommonEffectUtils::TryApplyDamageToActor(EffectContext.ReceivingActor, EffectContext.InstigatingActor, HitReactEvent.DamageTaken, HitReactEvent);
}

void ACommonStatModifierEffect::Internal_HealthMaxWounds(float ModifiedStatValue)
{
	UCommonEffectUtils::TryAddMaxWoundsToActor(EffectContext.ReceivingActor, ModifiedStatValue);
	AActor* ReceivingActor = EffectContext.ReceivingActor;
	ReversalFunc = [ModifiedStatValue, ReceivingActor]
	{
		UCommonEffectUtils::TryAddMaxWoundsToActor(ReceivingActor, ModifiedStatValue);
	};
}

void ACommonStatModifierEffect::Internal_HealthHeal(float ModifiedStatValue) const
{
	UCommonEffectUtils::TryApplyHealToActor(EffectContext.ReceivingActor, EffectContext.InstigatingActor, ModifiedStatValue);
}
