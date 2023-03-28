// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/InteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/InteractUtils.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	Internal_AddOwnerMeshesToArray();
	Internal_SetupInteractionTimeline();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(Timeline.IsPlaying())
	{
		Timeline.TickTimeline(DeltaTime);
	}
}

void UInteractionComponent::Internal_SetupInteractionTimeline()
{
	IInteractable* InteractableOwner = Cast<IInteractable>(GetOwner());
	if(!InteractableOwner)
	{
		return;
	}	
	InteractionEvent.AddRaw(InteractableOwner, &IInteractable::HandleInteractionStarted);
	Timeline.SetTimelineLength(InteractTime);
	Timeline.SetTimelineLengthMode(TL_TimelineLength);
	Timeline.SetLooping(false);
	
	FOnTimelineEvent InteractProgressFunction;
	InteractProgressFunction.BindDynamic(this, &UInteractionComponent::Internal_InteractionTick);
	Timeline.SetTimelinePostUpdateFunc(InteractProgressFunction);
	
	FOnTimelineEvent InteractFinishedEvent;
	InteractFinishedEvent.BindDynamic(this, &UInteractionComponent::Internal_InteractionFinished);
	Timeline.SetTimelineFinishedFunc(InteractFinishedEvent);

	SetComponentTickEnabled(false);
}


void UInteractionComponent::SwitchOutlineOnAllMeshes(bool bShouldOutline)
{
	for(UMeshComponent* CurrMesh : OwnerMeshes)
	{
		if(CurrMesh)
		{
			const int32 OutlineColorInt = UInteractUtils::GetOutlineInt(GetOwner());
			CurrMesh->SetRenderCustomDepth(bShouldOutline);
			CurrMesh->SetCustomDepthStencilValue(OutlineColorInt);
		}
	}
}

void UInteractionComponent::StartInteraction(AActor* InstigatingActor, bool bStartingInteraction)
{
	if(bInteractInstantly)
	{
		if(bStartingInteraction)
			InteractionEvent.Broadcast(FInteractionEventPayload(InstigatingActor));
		return;
	}

	if(bStartingInteraction)
	{
		CachedInstigatingActor = InstigatingActor;
		Timeline.IsPlaying() ? Timeline.Play() : Timeline.PlayFromStart();
	} else
	{
		CachedInstigatingActor = nullptr;
		Timeline.Reverse();
	}
	SetComponentTickEnabled(true);
}

void UInteractionComponent::Internal_AddOwnerMeshesToArray()
{
	if(const AActor* CurrOwner = GetOwner())
	{
		TSet<UActorComponent*> CurrComponents = CurrOwner->GetComponents();
		for(UActorComponent* CurrComp : CurrComponents)
		{
			if(!CurrComp)
			{
				continue;
			}
			
			if(UMeshComponent* CastedComp = Cast<UMeshComponent>(CurrComp))
			{
				OwnerMeshes.Add(CastedComp);
			}
		}
	}
}

void UInteractionComponent::Internal_InteractionTick()
{
	// Option for callback to UI events or FX to item being interacted with with a progress meter or something like that
}

void UInteractionComponent::Internal_InteractionFinished()
{
	// If the instigator has been set, we want to finish the this interaction because the timeline is in the forward direction.
	// If not, the timeline was moving backward (and resetting) so ignore this
	if(CachedInstigatingActor)
	{
		InteractionEvent.Broadcast(FInteractionEventPayload(CachedInstigatingActor));	
	}
	SetComponentTickEnabled(false);
}