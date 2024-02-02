﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/MaterialDissolverComponent.h"


UMaterialDissolverComponent::UMaterialDissolverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	MeshComponent = nullptr;
}

void UMaterialDissolverComponent::InitDissolveableMesh(UMeshComponent* InMesh)
{
	MeshComponent = InMesh;
	InitDissolveTimeline();
	Internal_SetDissolveMeshValues(StartingDissolveValue);
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

void UMaterialDissolverComponent::Internal_TimelineDissolveUpdate()
{
	Internal_SetDissolveMeshValues(DissolveTimeline.GetPlaybackPosition() / DissolveTimeline.GetTimelineLength());
}

void UMaterialDissolverComponent::Internal_SetDissolveMeshValues(float Value)
{
	if(DissolveDynamicMaterialInstances.Num() > 0)
	{
		for(TSoftObjectPtr<UMaterialInstanceDynamic> CurrInst : DissolveDynamicMaterialInstances)
		{
			if(CurrInst.IsValid())
			{
				CurrInst->SetScalarParameterValue(DissolveParameterName, Value);
			}
		}		
	}
}

void UMaterialDissolverComponent::InitDissolveTimeline()
{	
	if(MeshComponent.IsValid())
	{
		for(int i = 0; i<MeshComponent->GetMaterials().Num(); i++)
		{
			DissolveDynamicMaterialInstances.Add(MeshComponent->CreateDynamicMaterialInstance(i));
		}
	}

	DissolveTimeline.SetLooping(false);
	
	FOnTimelineEvent DissolveProgressFunction;
	DissolveProgressFunction.BindDynamic(this, &ThisClass::Internal_TimelineDissolveUpdate);
	DissolveTimeline.SetTimelinePostUpdateFunc(DissolveProgressFunction);
	
	FOnTimelineEvent DissolveFinishedEvent;
	DissolveFinishedEvent.BindDynamic(this, &ThisClass::Internal_TimelineDissolveEnd);
	DissolveTimeline.SetTimelineFinishedFunc(DissolveFinishedEvent);
	
	SetComponentTickEnabled(false);
}
