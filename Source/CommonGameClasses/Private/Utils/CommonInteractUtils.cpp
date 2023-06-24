// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CommonInteractUtils.h"

#include "ActorComponent/InteractionComponent.h"
#include "Character/CommonPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "Utils/CommonCombatUtils.h"
#include "Utils/CommonCoreUtils.h"

bool UCommonInteractUtils::AreActorsAllies(const AActor* FirstActor, const AActor* SecondActor)
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

bool UCommonInteractUtils::AreActorsEnemies(AActor* FirstActor, AActor* SecondActor)
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

bool UCommonInteractUtils::IsActorDestructible(const AActor* FirstActor)
{
	return GetAffiliationOfActor(FirstActor) == EAffiliation::Destructible;
}

bool UCommonInteractUtils::IsActorNeutral(const AActor* FirstActor)
{
	return GetAffiliationOfActor(FirstActor) == EAffiliation::Neutral;
}

EAffiliation UCommonInteractUtils::GetAffiliationOfActor(const AActor* InActor)
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
EAffiliation UCommonInteractUtils::GetAffiliationRelatedToPlayerCharacter(AActor* ContextActor)
{
	if(IsActorNeutral(ContextActor))
	{
		return EAffiliation::Neutral;
	}

	if(IsActorDestructible(ContextActor))
	{
		return EAffiliation::Enemies;
	}

	AActor* PlayerCharacter = UCommonCoreUtils::GetCommonPlayerCharacter(ContextActor);
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


int32 UCommonInteractUtils::GetOutlineInt(const AActor* InActor)
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