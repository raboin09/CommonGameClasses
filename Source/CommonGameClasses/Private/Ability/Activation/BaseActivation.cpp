#include "Ability/Activation/BaseActivation.h"

void UBaseActivation::InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent)
{
	MeshComponentRef = OwnerMeshComponent;
}
