#include "Actors/CommonInteractableActor.h"
#include "ActorComponent/InteractionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/RotatingMovementComponent.h"

ACommonInteractableActor::ACommonInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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
	
	bDiesAfterOverlap = true;
	DefaultGameplayTags.Add(TAG_STATE_ACTIVE);
	DeathBuffer = 2.f;
}

void ACommonInteractableActor::SwitchOutlineOnMesh(bool bShouldOutline)
{
	if(InteractionComponent)
	{
		InteractionComponent->SwitchOutlineOnAllMeshes(bShouldOutline);
	}
	K2_HandleMeshOutlining(bShouldOutline);
}

void ACommonInteractableActor::InitiateInteractionWithActor(AActor* InstigatingActor, bool bStartingInteraction)
{
	K2_HandleInteractionInitiated(InstigatingActor);
	if(InteractionComponent)
	{
		InteractionComponent->StartInteraction(InstigatingActor, bStartingInteraction);
	}
}

void ACommonInteractableActor::HandleInteractionStarted(const FInteractionEventPayload& InteractionEventPayload)
{
	K2_HandleInteractionStarted(InteractionEventPayload.InstigatingActor);
}

void ACommonInteractableActor::K2_HandleOverlapEvent_Implementation(AActor* OtherActor, const FHitResult& HitResult)
{
	if(ACharacter* CastedChar = Cast<ACharacter>(OtherActor))
	{
		if(CanPickup(CastedChar))
		{
			ConsumePickup(CastedChar);
			if(bDiesAfterOverlap)
			{
				PickupBase->SetVisibility(false);
			}
			Super::K2_HandleOverlapEvent_Implementation(OtherActor, HitResult);
		}
	}
}

void ACommonInteractableActor::ConsumePickup(ACharacter* ConsumingChar)
{
	K2_HandleConsumePickup(ConsumingChar);
}

void ACommonInteractableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(RotatingMovementComponent)
	{
		RotatingMovementComponent->SetUpdatedComponent(GetMesh());	
	}
}