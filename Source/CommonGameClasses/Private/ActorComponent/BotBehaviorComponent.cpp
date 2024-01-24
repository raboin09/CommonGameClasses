#include "ActorComponent/BotBehaviorComponent.h"

#include "ActorComponent/ActorAssetManagerComponent.h"

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

