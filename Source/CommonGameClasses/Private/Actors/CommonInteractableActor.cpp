#include "Actors/CommonInteractableActor.h"
#include "ActorComponent/InteractionComponent.h"
#include "Types/CoreTypes.h"

ACommonInteractableActor::ACommonInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void ACommonInteractableActor::BeginPlay()
{
	Super::BeginPlay();	
	Timeline.SetTimelineLength(InteractTime);
	Timeline.SetTimelineLengthMode(TL_TimelineLength);
	Timeline.SetLooping(false);
	
	FOnTimelineEvent InteractProgressFunction;
	InteractProgressFunction.BindDynamic(this, &ACommonInteractableActor::Internal_InteractionTick);
	Timeline.SetTimelinePostUpdateFunc(InteractProgressFunction);
	
	FOnTimelineEvent InteractFinishedEvent;
	InteractFinishedEvent.BindDynamic(this, &ACommonInteractableActor::Internal_InteractionFinished);
	Timeline.SetTimelineFinishedFunc(InteractFinishedEvent);	
}

void ACommonInteractableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Timeline.IsPlaying())
	{
		Timeline.TickTimeline(DeltaSeconds);
	}
}

void ACommonInteractableActor::Internal_InteractionTick()
{

}

void ACommonInteractableActor::Internal_InteractionFinished()
{
	// If the instigator has been set, we want to finish the this interaction. If not, ignore cooldown
	if(CachedInstigatingActor)
	{
		K2_HandleInteraction(CachedInstigatingActor);	
	}
}

void ACommonInteractableActor::SwitchOutlineOnMesh(bool bShouldOutline)
{
	if(InteractionComponent)
	{
		InteractionComponent->SwitchOutlineOnAllMeshes(bShouldOutline);
	}
	K2_HandleMeshOutlining(bShouldOutline);
}

void ACommonInteractableActor::InteractWithActor(AActor* InstigatingActor, bool bStartingInteraction)
{
	if(bInteractInstantly)
	{
		if(bStartingInteraction)
			K2_HandleInteraction(InstigatingActor);
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
}
