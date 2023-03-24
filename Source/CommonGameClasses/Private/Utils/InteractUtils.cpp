// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/InteractUtils.h"
#include "Utils/CoreUtils.h"
#include "Character/CommonPlayerCharacter.h"
#include "API/Interactable.h"
#include "GameFramework/Character.h"

bool UInteractUtils::AreActorsAllies(TScriptInterface<IInteractable> FirstActor, AActor* SecondActor)
{
	const IInteractable* CastedOwner = Cast<IInteractable>(SecondActor);
	if(FirstActor && CastedOwner)
	{
		if(IsActorNeutral(FirstActor))
		{
			return false;
		}

		if(IsActorNeutral(SecondActor))
		{
			return false;
		}
		
		return FirstActor->GetAffiliation() == CastedOwner->GetAffiliation();
	}
	return false;
}

bool UInteractUtils::AreActorsEnemies(TScriptInterface<IInteractable> FirstActor, AActor* SecondActor)
{
	const IInteractable* CastedOwner = Cast<IInteractable>(SecondActor);
	if(FirstActor && CastedOwner)
	{
		if(IsActorNeutral(FirstActor))
		{
			return false;
		}

		if(IsActorNeutral(SecondActor))
		{
			return false;
		}
		
		return FirstActor->GetAffiliation() != CastedOwner->GetAffiliation();
	}
	return false;
}

bool UInteractUtils::AreActorsAllies(AActor* FirstActor, AActor* SecondActor)
{
	const IInteractable* CastedChar = Cast<IInteractable>(FirstActor);
	const IInteractable* CastedOwner = Cast<IInteractable>(SecondActor);
	if(CastedChar && CastedOwner)
	{
		if(IsActorNeutral(FirstActor))
		{
			return false;
		}
  
		if(IsActorNeutral(SecondActor))
		{
			return false;
		}
		
		return CastedChar->GetAffiliation() == CastedOwner->GetAffiliation();
	}
	return false;
}

bool UInteractUtils::AreActorsEnemies(AActor* FirstActor, AActor* SecondActor)
{
	const IInteractable* CastedChar = Cast<IInteractable>(FirstActor);
	const IInteractable* CastedOwner = Cast<IInteractable>(SecondActor);
	if(CastedChar && CastedOwner)
	{
		if(IsActorNeutral(FirstActor))
		{
			return false;
		}

		if(IsActorNeutral(SecondActor))
		{
			return false;
		}
		
		return CastedChar->GetAffiliation() != CastedOwner->GetAffiliation();
	}
	return false;
}

bool UInteractUtils::IsActorNeutral(AActor* FirstActor)
{
	if(const IInteractable* CastedChar = Cast<IInteractable>(FirstActor))
	{
		return CastedChar->GetAffiliation() == EAffiliation::Neutral;
	}
	return false;
}

bool UInteractUtils::IsActorNeutral(TScriptInterface<IInteractable> FirstActor)
{
	if(FirstActor)
	{
		return FirstActor->GetAffiliation() == EAffiliation::Neutral;
	}
	return false;
}

EAffiliation UInteractUtils::GetAffiliationRelatedToPlayerCharacter(AActor* ContextActor)
{
	if(IsActorNeutral(ContextActor))
	{
		return EAffiliation::Neutral;
	}

	ACommonPlayerCharacter* PlayerCharacter = UCoreUtils::GetCommonPlayerCharacter(ContextActor);
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

int32 UInteractUtils::GetOutlineInt(AActor* InActor)
{
	switch(GetAffiliationRelatedToPlayerCharacter(InActor))
	{
		case EAffiliation::Allies: return OUTLINE_COLOR_GREEN;
		case EAffiliation::Enemies: return OUTLINE_COLOR_RED;
		case EAffiliation::Neutral: return OUTLINE_COLOR_GRAY;
		default: return OUTLINE_COLOR_GRAY;
	}
}