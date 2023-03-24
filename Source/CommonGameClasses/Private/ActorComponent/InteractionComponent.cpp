// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent/InteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/InteractUtils.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	Internal_AddOwnerMeshesToArray();
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