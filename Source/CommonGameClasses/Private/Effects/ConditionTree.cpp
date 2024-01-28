// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/ConditionTree.h"

#include "ActorComponent/GameplayTagComponent.h"
#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/ShieldEnergyComponent.h"

TWeakObjectPtr<AActor> UConditionExpression::GetPropertyOwner(EPropertyOwner PropertyOwner, const FEffectContext& InEffectContext) const
{
	switch (PropertyOwner) {
		case EPropertyOwner::Instigator: return InEffectContext.InstigatingActor;
		case EPropertyOwner::ReceivingActor: return InEffectContext.ReceivingActor;
		default: return nullptr;
	}
}

bool UConditionTreeNode::AreConditionsTrue(const FEffectContext& InEffectContext) const
{
	for(const UConditionExpression* ChildCondition : ChildConditions)
	{
		if(!ChildCondition)
		{
			continue;
		}
		if(!ChildCondition->AreConditionsTrue(InEffectContext))
		{
			return false;
		}
	}
	return true;
}

bool UFloatLeafConditionNode::AreConditionsTrue(const FEffectContext& InEffectContext) const
{
	const float PropertyValue = GetProperty(InEffectContext);
	switch (Operator) {
		case EMathLeafOperator::Equal: return PropertyValue == Value;
		case EMathLeafOperator::NotEqual: return PropertyValue != Value;
		case EMathLeafOperator::GreaterThan: return PropertyValue > Value;
		case EMathLeafOperator::GreaterThanOrEqual: return PropertyValue >= Value;
		case EMathLeafOperator::LessThan: return PropertyValue < Value;
		case EMathLeafOperator::LessThanOrEqual: return PropertyValue <= Value;
		default: return false;
	}
}

float UFloatLeafConditionNode::GetProperty(const FEffectContext& InEffectContext) const
{
	const TWeakObjectPtr<AActor> PropertyOwnerActor = GetPropertyOwner(PropertyOwner, InEffectContext);
	if(!PropertyOwnerActor.IsValid())
	{
		return 0.f;
	}
	
	switch (FloatProperty)
	{
	case EFloatOperand::CurrentHealth:
		{
			if(const UHealthComponent* HealthComponent = PropertyOwnerActor->GetComponentByClass<UHealthComponent>())
			{
				return HealthComponent->GetCurrentHealth();
			}
			return 0.f;
		}
	case EFloatOperand::CurrentShield:
		{
			if(const UShieldEnergyComponent* ShieldEnergyComponent = PropertyOwnerActor->GetComponentByClass<UShieldEnergyComponent>())
			{
				return ShieldEnergyComponent->GetCurrentEnergy();
			}
			return 0.f;
		}
	default:
		return 0.f;
	}
}

bool UGameplayTagConditionNode::AreConditionsTrue(const FEffectContext& InEffectContext) const
{
	TWeakObjectPtr<AActor> TagActor = GetPropertyOwner(PropertyOwner, InEffectContext);
	if(!TagActor.IsValid())
	{
		return false;
	}
	
	switch (Operator) {
		case ETagOperator::HasTagsAny: return UGameplayTagComponent::ActorHasAnyGameplayTags(TagActor.Get(), ConditionTags, bExactMatch);
		case ETagOperator::HasTagsAll: return UGameplayTagComponent::ActorHasAnyGameplayTags(TagActor.Get(), ConditionTags, bExactMatch);
		default: return false;
	}
}