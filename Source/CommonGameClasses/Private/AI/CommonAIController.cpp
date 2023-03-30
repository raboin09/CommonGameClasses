
#include "AI/CommonAIController.h"
#include "Character/CommonAICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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
}

void ACommonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AIPawn = Cast<ACommonAICharacter>(InPawn);
	if(!AIPawn)
	{
		return;
	}
	InitAIComponents(AIPawn->GetDefaultBehavior());
	InitPerceptionComponents();
}

void ACommonAIController::OnUnPossess()
{
	BehaviorTreeComponent->StopTree();
	Super::OnUnPossess();
}

void ACommonAIController::Internal_OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

}

void ACommonAIController::InitAIComponents(UBehaviorTree* BehaviorTree)
{
	if(!AIPawn || !BehaviorTree)
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
	if(AIPerceptionComponent)
	{
		AIPerceptionComponent->OnPerceptionUpdated.RemoveAll(this);
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ACommonAIController::Internal_OnPerceptionUpdated);
	}
}
