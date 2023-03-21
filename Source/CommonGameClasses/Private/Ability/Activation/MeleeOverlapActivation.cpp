
#include "Ability/Activation/MeleeOverlapActivation.h"

AMeleeOverlapActivation::AMeleeOverlapActivation()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMeleeOverlapActivation::BeginPlay()
{
	Super::BeginPlay();	
}

void AMeleeOverlapActivation::Activate(const FTriggerEventPayload& TriggerEventPayload)
{
	
}

void AMeleeOverlapActivation::Deactivate()
{
	
}
