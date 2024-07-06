#include "ActorComponent/BotBehaviorComponent.h"

UBotBehaviorComponent::UBotBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBotBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBotBehaviorComponent::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
} 

