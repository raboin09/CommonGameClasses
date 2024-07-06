
#include "ActorComponent/MountManagerComponent.h"

UMountManagerComponent::UMountManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMountManagerComponent::AssignNewMountable(UObject* InMountableObject, const FHitResult& InHitResult)
{
	if(IMountable* Mount = Cast<IMountable>(InMountableObject))
	{
		Mount->OccupyMount(this, FVector::ZeroVector, InHitResult.ImpactNormal);
		TScriptInterface<IMountable> NewCover;
		NewCover.SetObject(InMountableObject);
		NewCover.SetInterface(Mount);
		CurrentMount = NewCover;
	}
}