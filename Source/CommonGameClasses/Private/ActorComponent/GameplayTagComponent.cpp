// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/GameplayTagComponent.h"
#include "BlueprintGameplayTagLibrary.h"
#include "Character/CommonCharacter.h"

void UGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();
	if(ITaggable* Taggable = Cast<ITaggable>(GetOwner()))
	{
		GameplayTagAddedEvent.AddRaw(Taggable, &ITaggable::HandleTagAdded);
		GameplayTagRemovedEvent.AddRaw(Taggable, &ITaggable::HandleTagRemoved);	
	}
}

void UGameplayTagComponent::AddTag(const FGameplayTag& TagToAdd)
{
	GameplayTagContainer.AddTag(TagToAdd);
	GameplayTagAddedEvent.Broadcast(TagToAdd);
}

void UGameplayTagComponent::RemoveTag(const FGameplayTag& TagToRemove)
{
	GameplayTagContainer.RemoveTag(TagToRemove);
	GameplayTagRemovedEvent.Broadcast(TagToRemove);
}


////////////////////////////////////////////
// Static Functions
////////////////////////////////////////////

void UGameplayTagComponent::AddTagToActor(AActor* InActor, const FGameplayTag& InTag, UGameplayTagComponent* GameplayTagComponent)
{
	if(!GameplayTagComponent)
	{
		if(UGameplayTagComponent* FoundComponent = GetGameplayTagComponentFromActor(InActor))
		{
			FoundComponent->AddTag(InTag);
		}
	} else
	{
		GameplayTagComponent->AddTag(InTag);
	}
}

void UGameplayTagComponent::AddTagsToActor(AActor* InActor, const TArray<FGameplayTag>& InTags)
{
	if(UGameplayTagComponent* FoundComponent = GetGameplayTagComponentFromActor(InActor))
	{
		for(FGameplayTag Tag : InTags)
		{
			AddTagToActor(InActor, Tag, FoundComponent);
		}
	}
}

void UGameplayTagComponent::RemoveTagsFromActor(AActor* InActor, const TArray<FGameplayTag>& InTags)
{
	if(UGameplayTagComponent* FoundComponent = GetGameplayTagComponentFromActor(InActor))
	{
		for(FGameplayTag Tag : InTags)
		{
			RemoveTagFromActor(InActor, Tag, FoundComponent);
		}
	}
}

void UGameplayTagComponent::RemoveTagFromActor(AActor* InActor, const FGameplayTag& InTag, UGameplayTagComponent* GameplayTagComponent)
{
	if(!GameplayTagComponent)
	{
		if(UGameplayTagComponent* FoundComponent = GetGameplayTagComponentFromActor(InActor))
		{
			FoundComponent->RemoveTag(InTag);
		}		
	} else
	{
		GameplayTagComponent->RemoveTag(InTag);
	}
}

UGameplayTagComponent* UGameplayTagComponent::GetGameplayTagComponentFromActor(AActor* InActor)
{
	if(!InActor)
	{
		return nullptr;
	}
	return InActor->FindComponentByClass<UGameplayTagComponent>();
}

bool UGameplayTagComponent::ComponentHasNameTag(UActorComponent* InComp, FName InTag)
{
	if(InComp)
	{
		return InComp->ComponentHasTag(InTag);
	}
	return false;
}

bool UGameplayTagComponent::ActorHasAnyGameplayTags(AActor* InActor, TArray<FGameplayTag> InTags, bool bExact)
{
	if(UGameplayTagComponent* FoundComponent = GetGameplayTagComponentFromActor(InActor))
	{
		return UBlueprintGameplayTagLibrary::HasAnyTags(FoundComponent->GetTagContainer(), FGameplayTagContainer::CreateFromArray(InTags), bExact);
	}
	return false;
}

bool UGameplayTagComponent::ActorHasAllGameplayTags(AActor* InActor, TArray<FGameplayTag> InTags, bool bExact)
{
	if(UGameplayTagComponent* FoundComponent = GetGameplayTagComponentFromActor(InActor))
	{
		return UBlueprintGameplayTagLibrary::HasAllTags(FoundComponent->GetTagContainer(), FGameplayTagContainer::CreateFromArray(InTags), bExact);
	}
	return false;
}

bool UGameplayTagComponent::ActorHasGameplayTag(AActor* InActor, FGameplayTag InTag, bool bExact)
{
	if(UGameplayTagComponent* FoundComponent = GetGameplayTagComponentFromActor(InActor))
	{
		return UBlueprintGameplayTagLibrary::HasTag(FoundComponent->GetTagContainer(), InTag, bExact);
	}
	return false;
}