#include "Ability/Activation/BaseActivation.h"

#include "Utils/CommonEffectUtils.h"

void UBaseActivation::InitActivationMechanism(TWeakObjectPtr<UMeshComponent> OwnerMeshComponent)
{
	MeshComponentRef = OwnerMeshComponent;
}

void UBaseActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	UCommonEffectUtils::ApplyEffectsToActor(OwnerApplyEffectsOnActivation, GetInstigator());
}

void UBaseActivation::Deactivate()
{
	UCommonEffectUtils::ApplyEffectsToActor(OwnerApplyEffectsOnDeactivation, GetInstigator());
}
