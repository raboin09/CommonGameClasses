// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/InteractUtils.h"

#include "ActorComponent/InteractionComponent.h"
#include "Character/CommonPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "Utils/CombatUtils.h"
#include "Utils/CoreUtils.h"

bool UInteractUtils::AreActorsAllies(const AActor* FirstActor, const AActor* SecondActor)
{
	if(IsActorNeutral(FirstActor))
	{
		return false;
	}

	if(IsActorNeutral(SecondActor))
	{
		return false;
	}	
	return GetAffiliationOfActor(FirstActor) == GetAffiliationOfActor(SecondActor);
}

bool UInteractUtils::AreActorsEnemies(AActor* FirstActor, AActor* SecondActor)
{
	if(IsActorNeutral(FirstActor))
	{
		return false;
	}

	if(IsActorNeutral(SecondActor))
	{
		return false;
	}	
	return GetAffiliationOfActor(FirstActor) != GetAffiliationOfActor(SecondActor);
}

bool UInteractUtils::IsActorDestructible(const AActor* FirstActor)
{
	return GetAffiliationOfActor(FirstActor) == EAffiliation::Destructible;
}

bool UInteractUtils::IsActorNeutral(const AActor* FirstActor)
{
	return GetAffiliationOfActor(FirstActor) == EAffiliation::Neutral;
}

EAffiliation UInteractUtils::GetAffiliationOfActor(const AActor* InActor)
{
	if(!InActor)
	{
		return EAffiliation::None;
	}
	if(const UInteractionComponent* InteractionComponent = InActor->FindComponentByClass<UInteractionComponent>())
	{
		return InteractionComponent->Affiliation;
	}
	return EAffiliation::None;
}
EAffiliation UInteractUtils::GetAffiliationRelatedToPlayerCharacter(AActor* ContextActor)
{
	if(IsActorNeutral(ContextActor))
	{
		return EAffiliation::Neutral;
	}

	if(IsActorDestructible(ContextActor))
	{
		return EAffiliation::Enemies;
	}

	AActor* PlayerCharacter = UCoreUtils::GetCommonPlayerCharacter(ContextActor);
	if(AreActorsAllies(PlayerCharacter, ContextActor))
	{
		return EAffiliation::Allies;
	}

	if(AreActorsEnemies(PlayerCharacter, ContextActor))
	{
		return EAffiliation::Enemies;
	}
	return EAffiliation::Neutral;
}


int32 UInteractUtils::GetOutlineInt(const AActor* InActor)
{
	switch(GetAffiliationOfActor(InActor))
	{
		case EAffiliation::Allies: return OUTLINE_COLOR_GREEN;
		case EAffiliation::Enemies: return OUTLINE_COLOR_RED;
		case EAffiliation::Neutral: return OUTLINE_COLOR_GRAY;
		case EAffiliation::Destructible: return OUTLINE_COLOR_RED;
		default: return OUTLINE_COLOR_GRAY;
	}
}