#include "ActorComponent/BotBehaviorComponent.h"

UBotBehaviorComponent::UBotBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBotBehaviorComponent::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
} 

