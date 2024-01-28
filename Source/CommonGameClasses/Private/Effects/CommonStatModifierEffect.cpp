#include "Effects/CommonStatModifierEffect.h"

#include "ActorComponent/GameplayTagComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/CommonCombatUtils.h"
#include "Utils/CommonEffectUtils.h"

UBaseStatsModifierData::UBaseStatsModifierData()
{
	BaseModifierValue = 0;
	HitImpulse = EHitReactType::None;
	DeathImpulse = EHitReactType::None;
}

ACommonStatModifierEffect::ACommonStatModifierEffect()
{
	StatEffectData = CreateDefaultSubobject<UBaseStatsModifierData>(TEXT("StatEffectData"));
}

bool ACommonStatModifierEffect::TryActivateEffect()
{
	if(CanActivateEffect())
	{
		K2N_ApplyStatChange(CalculateModifierValues());	
	}
	return Super::TryActivateEffect();
}

void ACommonStatModifierEffect::DestroyEffect()
{
	Super::DestroyEffect();
	if(EffectData->InitializationData.bShouldReverseChangesAfterDestroy && ReversalFunc)
	{
		ReversalFunc();
	}
}

void ACommonStatModifierEffect::K2N_ApplyStatChange_Implementation(float ModifiedStatValue)
{
	switch (StatEffectData->StatToModify) {
		case EEffectStatType::HealShield:
			Internal_ShieldHeal(ModifiedStatValue);
			break;
		case EEffectStatType::MaxWounds:
			Internal_HealthMaxWounds(ModifiedStatValue);
			break;
		case EEffectStatType::HealHealth:
			Internal_HealthHeal(ModifiedStatValue);
			break;
		case EEffectStatType::Damage_All:
			Internal_AllDamage(ModifiedStatValue);
			break;
		case EEffectStatType::MoveSpeed:
			Internal_MoveSpeedStatChange(ModifiedStatValue);
			break;
		case EEffectStatType::Damage_Health:
			Internal_HealthDamage(ModifiedStatValue);
			break;
		case EEffectStatType::Damage_Shield:
			Internal_ShieldDamage(ModifiedStatValue);
			break;
		default:
			;
	}
}

float ACommonStatModifierEffect::CalculateHeadshotDamage(float ModifiedStatValue) const
{
	if(StatEffectData->bAddDamageForHeadshots && UCommonCombatUtils::IsBoneNameHead(EffectContext.SurfaceHit.BoneName))
	{
		return ModifiedStatValue * StatEffectData->HeadshotModifier;
	}
	return ModifiedStatValue;
}

float ACommonStatModifierEffect::CalculateModifierValues() const
{
	TArray<FGameplayTag> ModifierKeys;
	EffectData->EffectModifiers.GetKeys(ModifierKeys);
	float ModifiedValue = StatEffectData->BaseModifierValue;
	for(const FGameplayTag& CurrentTag : ModifierKeys)
	{
		CalculateModifier(*EffectData->EffectModifiers.Find(CurrentTag), CurrentTag, ModifiedValue);
	}
	return ModifiedValue;
}

void ACommonStatModifierEffect::CalculateModifier(const FModifierExpression& ModifierExpression, const FGameplayTag& ModifierTag, float& ModifiedBaseValue) const
{
	TWeakObjectPtr<AActor> ModifierActor = nullptr;
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

	if(!UGameplayTagComponent::ActorHasGameplayTag(ModifierActor.Get(), ModifierTag))
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

void ACommonStatModifierEffect::Internal_MoveSpeedStatChange(float ModifiedStatValue)
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

void ACommonStatModifierEffect::Internal_AllDamage(float ModifiedStatValue) const
{
	const FDamageHitReactEvent& HitReactEvent = Internal_GenerateHitReactEvent(ModifiedStatValue);
	UCommonEffectUtils::TryApplyDamageToActor(EffectContext.ReceivingActor.Get(), EffectContext.InstigatingActor.Get(), HitReactEvent.DamageTaken, HitReactEvent);
}

void ACommonStatModifierEffect::Internal_HealthDamage(float ModifiedStatValue) const
{
	const FDamageHitReactEvent& HitReactEvent = Internal_GenerateHitReactEvent(ModifiedStatValue);
	UCommonEffectUtils::TryApplyHealthDamageToActor(EffectContext.ReceivingActor.Get(), EffectContext.InstigatingActor.Get(), HitReactEvent.DamageTaken, HitReactEvent);
}

void ACommonStatModifierEffect::Internal_ShieldDamage(float ModifiedStatValue) const
{
	const FDamageHitReactEvent& HitReactEvent = Internal_GenerateHitReactEvent(ModifiedStatValue);
	UCommonEffectUtils::TryApplyShieldDamageToActor(EffectContext.ReceivingActor.Get(), EffectContext.InstigatingActor.Get(), HitReactEvent.DamageTaken, HitReactEvent);
}

void ACommonStatModifierEffect::Internal_HealthMaxWounds(float ModifiedStatValue)
{
	UCommonEffectUtils::TryAddMaxWoundsToActor(EffectContext.ReceivingActor.Get(), ModifiedStatValue);
	TWeakObjectPtr<AActor> ReceivingActor = EffectContext.ReceivingActor;
	ReversalFunc = [ModifiedStatValue, ReceivingActor]
	{
		UCommonEffectUtils::TryAddMaxWoundsToActor(ReceivingActor.Get(), ModifiedStatValue);
	};
}

FDamageHitReactEvent ACommonStatModifierEffect::Internal_GenerateHitReactEvent(float ModifiedStatValue) const
{
	FDamageHitReactEvent HitReactEvent;
	HitReactEvent.DamageTaken = CalculateHeadshotDamage(ModifiedStatValue);
	const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(EffectContext.SurfaceHit.TraceStart, EffectContext.ReceivingActor->GetActorLocation());
	HitReactEvent.HitDirection = LookAt.Vector().GetSafeNormal();
	HitReactEvent.HitResult = EffectContext.SurfaceHit;
	HitReactEvent.DeathReactType = StatEffectData->DeathImpulse;
	HitReactEvent.HitReactType = StatEffectData->HitImpulse;
	return HitReactEvent;
}

void ACommonStatModifierEffect::Internal_HealthHeal(float ModifiedStatValue) const
{
	UCommonEffectUtils::TryApplyHealthHealToActor(EffectContext.ReceivingActor.Get(), EffectContext.InstigatingActor.Get(), ModifiedStatValue);
}

void ACommonStatModifierEffect::Internal_ShieldHeal(float ModifiedStatValue) const
{
	UCommonEffectUtils::TryApplyShieldHealToActor(EffectContext.ReceivingActor.Get(), EffectContext.InstigatingActor.Get(), ModifiedStatValue);
}
