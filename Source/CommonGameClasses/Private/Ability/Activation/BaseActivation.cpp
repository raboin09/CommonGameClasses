#include "Ability/Activation/BaseActivation.h"

void UBaseActivation::InitActivationMechanism(UMeshComponent* OwnerMeshComponent)
{
	MeshComponentRef = OwnerMeshComponent;
}
