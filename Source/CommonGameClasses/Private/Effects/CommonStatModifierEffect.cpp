#include "Effects/CommonStatModifierEffect.h"

#include "ActorComponent/CommonCharacterMovementComponent.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/ShieldEnergyComponent.h"
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
	if(!EffectContext.ReceivingActor.IsValid())
	{
		return;
	}
	
	switch (StatEffectData->StatToModify) {
		case EEffectStatType::HealShield:
			Internal_ShieldHeal(ModifiedStatValue);
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
	if(UCommonCharacterMovementComponent* MovementComponent = EffectContext.ReceivingActor->FindComponentByClass<UCommonCharacterMovementComponent>())
	{
		// TODO Find a way to only revert modifier instead of reverting to 1.f
		MovementComponent->SetWalkSpeedRatio(ModifiedStatValue);
		ReversalFunc = [MovementComponent]
		{
			MovementComponent->SetWalkSpeedRatio(1.0);
		};
	}
}

void ACommonStatModifierEffect::Internal_AllDamage(float ModifiedStatValue) const
{
	const FDamageHitReactEvent& HitReactEvent = Internal_GenerateHitReactEvent(ModifiedStatValue);
	if(UShieldEnergyComponent* ShieldEnergyComponent = EffectContext.ReceivingActor.Get()->FindComponentByClass<UShieldEnergyComponent>())
	{
		ModifiedStatValue = ShieldEnergyComponent->TakeShieldDamage(ModifiedStatValue, EffectContext.InstigatingActor.Get(), HitReactEvent);
	}

	if(UHealthComponent* HealthComponent = EffectContext.ReceivingActor.Get()->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->TakeDamage(ModifiedStatValue, EffectContext.InstigatingActor.Get(), HitReactEvent);
	}
}

void ACommonStatModifierEffect::Internal_HealthDamage(float ModifiedStatValue) const
{
	const FDamageHitReactEvent& HitReactEvent = Internal_GenerateHitReactEvent(ModifiedStatValue);
	if(UHealthComponent* HealthComponent = EffectContext.ReceivingActor->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->TakeDamage(ModifiedStatValue, EffectContext.InstigatingActor.Get(), HitReactEvent);
	}
}

void ACommonStatModifierEffect::Internal_ShieldDamage(float ModifiedStatValue) const
{
	const FDamageHitReactEvent& HitReactEvent = Internal_GenerateHitReactEvent(ModifiedStatValue);
	if(UShieldEnergyComponent* ShieldEnergyComponent = EffectContext.ReceivingActor->FindComponentByClass<UShieldEnergyComponent>())
	{
		ShieldEnergyComponent->TakeShieldDamage(ModifiedStatValue, EffectContext.InstigatingActor.Get(), HitReactEvent);
	}
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
	if(UHealthComponent* HealthComponent = EffectContext.ReceivingActor.Get()->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->ApplyHeal(ModifiedStatValue, EffectContext.InstigatingActor.Get());
	}
}

void ACommonStatModifierEffect::Internal_ShieldHeal(float ModifiedStatValue) const
{
	if(UShieldEnergyComponent* ShieldEnergyComponent = EffectContext.ReceivingActor->FindComponentByClass<UShieldEnergyComponent>())
	{
		ShieldEnergyComponent->ApplyShieldHeal(ModifiedStatValue);
	}
}
