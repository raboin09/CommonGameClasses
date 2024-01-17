// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Effects/ConditionTree.h"

#include "ActorComponent/HealthComponent.h"
#include "ActorComponent/ShieldEnergyComponent.h"

AActor* UConditionExpression::GetPropertyOwner(EPropertyOwner PropertyOwner, const FEffectContext& InEffectContext) const
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

bool UFloatLeafNode::AreConditionsTrue(const FEffectContext& InEffectContext) const
{
	switch (Operator) {
		case EMathLeafOperator::Equal: return GetProperty(InEffectContext) == Value;
		case EMathLeafOperator::NotEqual: return GetProperty(InEffectContext) != Value;
		case EMathLeafOperator::GreaterThan: return GetProperty(InEffectContext) > Value;
		case EMathLeafOperator::GreaterThanOrEqual: return GetProperty(InEffectContext) >= Value;
		case EMathLeafOperator::LessThan: return GetProperty(InEffectContext) < Value;
		case EMathLeafOperator::LessThanOrEqual: return GetProperty(InEffectContext) <= Value;
		default: return false;
	}
}

float UFloatLeafNode::GetProperty(const FEffectContext& InEffectContext) const
{
	const AActor* PropertyOwnerActor = GetPropertyOwner(PropertyOwner, InEffectContext);
	if(!PropertyOwnerActor)
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
