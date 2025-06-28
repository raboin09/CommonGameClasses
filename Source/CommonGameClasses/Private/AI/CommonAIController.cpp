
#include "AI/CommonAIController.h"
#include "ActorComponent/BotBehaviorComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACommonAIController::ACommonAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BrainComponent = BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerceptionComp"));
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = 2500.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerceptionComponent->ConfigureSense(*Sight);
	bAttachToPawn = true;
}

void ACommonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(!InPawn)
	{
		return;
	}
	
	BotBehaviorComponent = InPawn->FindComponentByClass<UBotBehaviorComponent>();
	if(!BotBehaviorComponent.IsValid())
	{
		return;
	}
	
	if(!BotBehaviorComponent->GetDefaultBehavior().IsValid())
	{
		// FLoadedBehaviorTreeEvent LoadedBehaviorTreeEvent = FLoadedBehaviorTreeEvent();
		// LoadedBehaviorTreeEvent.BindDynamic(this, &ThisClass::HandleBehaviorTreeLoaded);
		// ActorAssetManagerComponent->BPI_Async_LoadBehaviorTreeObject(BotBehaviorComponent->GetDefaultBehavior(), false, LoadedBehaviorTreeEvent);
	} else
	{
		HandleBehaviorTreeLoaded(BotBehaviorComponent->GetDefaultBehavior());
	}
}

void ACommonAIController::OnUnPossess()
{
	BehaviorTreeComponent->StopTree();
	Super::OnUnPossess();
}

FVector ACommonAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	const auto* FocusedPawn{Cast<APawn>(Actor)};
	if (IsValid(FocusedPawn))
	{
		return FocusedPawn->GetPawnViewLocation();
	}

	return Super::GetFocalPointOnActor(Actor);
}

void ACommonAIController::InitAIBehavior(UBehaviorTree* BehaviorTree) const
{
	if(!BehaviorTree)
	{
		return;
	}
	
	BehaviorTreeComponent->StopTree();
	if (BehaviorTree->BlackboardAsset)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}	
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void ACommonAIController::InitPerceptionComponents()
{
	if(AIPerceptionComponent && BotBehaviorComponent.IsValid())
	{
		AIPerceptionComponent->OnPerceptionUpdated.RemoveAll(this);
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(BotBehaviorComponent.Get(), &UBotBehaviorComponent::HandlePerceptionUpdated);
	}
}

void ACommonAIController::HandleBehaviorTreeLoaded(TSoftObjectPtr<UBehaviorTree> LoadedTree)
{
	InitPerceptionComponents();
	InitAIBehavior(LoadedTree.Get());
}
