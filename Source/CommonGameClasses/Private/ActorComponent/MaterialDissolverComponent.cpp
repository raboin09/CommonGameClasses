// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/MaterialDissolverComponent.h"


UMaterialDissolverComponent::UMaterialDissolverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Load Dissolve Curve here from BP
	
	DissolveParameterName = "Dissolve Amount";
	ColorParameterName = "Color";
}

void UMaterialDissolverComponent::InitDissolveableMesh(UMeshComponent* InMesh)
{
	MeshComponent = InMesh;
	InitDissolveTimeline();
	Internal_TimelineDissolveUpdate(StartingDissolveValue);
}

void UMaterialDissolverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(DissolveTimeline.IsPlaying())
	{
		DissolveTimeline.TickTimeline(DeltaTime);
	}
}

void UMaterialDissolverComponent::StartAppearTimeline()
{
	DissolveTimeline.IsPlaying() ? DissolveTimeline.Play() : DissolveTimeline.PlayFromStart();
	SetComponentTickEnabled(true);
}

void UMaterialDissolverComponent::StartDissolveTimeline()
{
	DissolveTimeline.Reverse();
	SetComponentTickEnabled(true);
}

void UMaterialDissolverComponent::Internal_TimelineDissolveEnd()
{
	SetComponentTickEnabled(false);
}

void UMaterialDissolverComponent::Internal_TimelineDissolveUpdate(float Value)
{
	if(DissolveDynamicMaterialInstances.Num() > 0)
	{
		for(UMaterialInstanceDynamic* CurrInst : DissolveDynamicMaterialInstances)
		{
			if(CurrInst)
			{
				CurrInst->SetScalarParameterValue(DissolveParameterName, Value);
			}
		}		
	}
}

void UMaterialDissolverComponent::InitDissolveTimeline()
{	
	if(MeshComponent)
	{
		for(int i = 0; i<MeshComponent->GetMaterials().Num(); i++)
		{
			DissolveDynamicMaterialInstances.Add(MeshComponent->CreateDynamicMaterialInstance(i));
		}
	}

	if(DissolveCurveFloat)
	{
		FOnTimelineFloat DissolveProgressFunction;
		DissolveProgressFunction.BindDynamic(this, &UMaterialDissolverComponent::Internal_TimelineDissolveUpdate);
		DissolveTimeline.AddInterpFloat(DissolveCurveFloat, DissolveProgressFunction);
		DissolveTimeline.SetLooping(false);
	}
	
	FOnTimelineEvent CoverLerpFinishedEvent;
	CoverLerpFinishedEvent.BindDynamic(this, &UMaterialDissolverComponent::Internal_TimelineDissolveEnd);
	DissolveTimeline.SetTimelineFinishedFunc(CoverLerpFinishedEvent);
}
