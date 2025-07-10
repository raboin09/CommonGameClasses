#include "Actors/CommonInteractableActor.h"
#include "ActorComponent/InteractionComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Types/CommonCoreTypes.h"

ACommonInteractableActor::ACommonInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComp->SetCollisionResponseToChannel(COMMON_TRACE_INTERACTION, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COMMON_OBJECT_TYPE_PROJECTILE, ECR_Ignore);
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	SetRootComponent(CollisionComp);

	PickupBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupBaseMesh"));
	PickupBase->SetupAttachment(RootComponent);
	PickupBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupBase->SetCollisionResponseToAllChannels(ECR_Ignore);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComponent"));
	RotatingMovementComponent->bRotationInLocalSpace = true;
	RotatingMovementComponent->RotationRate = FRotator(0.f, 90.f, 0.f);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->Affiliation = EAffiliation::InteractionActor;
	
	bDiesAfterOverlap = true;
	DefaultGameplayTags.Add(CommonStateTags::Active);
	DeathBuffer = 2.f;
}

void ACommonInteractableActor::HandleInteractionStarted(const FInteractionStartedEventPayload InteractionEventPayload)
{
	BPI_OnInteractionStarted(InteractionEventPayload);
}

void ACommonInteractableActor::HandleInteractionInitiated(const FInteractionInitiatedEventPayload InteractionEventPayload)
{
	BPI_OnInteractionInitiated(InteractionEventPayload);
}

void ACommonInteractableActor::HandleMeshOutlining(const FInteractionOutlinedEventPayload InteractionOutlineEventPayload)
{
	BPI_OnMeshOutlined(InteractionOutlineEventPayload);
}

void ACommonInteractableActor::HandleValidActorBeginOverlap(AActor* RequestingActor, const FHitResult& HitResult)
{
	StartInteraction(RequestingActor);
	if(bDiesAfterOverlap)
	{
		PickupBase->SetVisibility(false);
	}
	Super::HandleValidActorBeginOverlap(RequestingActor, HitResult);
}

void ACommonInteractableActor::StartInteraction(AActor* RequestingActor)
{
	BPI_OnInteractionSucceeded(RequestingActor);
}

void ACommonInteractableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(RotatingMovementComponent)
	{
		RotatingMovementComponent->SetUpdatedComponent(BPN_GetMesh());	
	}
}