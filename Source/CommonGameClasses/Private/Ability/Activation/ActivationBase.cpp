#include "Ability/Activation/ActivationBase.h"
#include "GameFramework/Character.h"

void AActivationBase::BeginPlay()
{
	Super::BeginPlay();
	InitAssignMeshComponentRef();
}

void AActivationBase::InitAssignMeshComponentRef()
{
	// Find the first non-null mesh component that we come across
	if(MeshType == EMeshType::InstigatorMesh)
	{
		// Check if Instigator is a char, if so then GetMesh(). Otherwise, find first skel mesh
		if(const ACharacter* CharRef = Cast<ACharacter>(GetInstigator()))
		{
			MeshComponentRef = CharRef->GetMesh();
		} else
		{
			MeshComponentRef = GetInstigator()->FindComponentByClass<USkeletalMeshComponent>();	
		}
	} else if (MeshType == EMeshType::AbilityMesh)
	{
		// Find the first SkeletalMesh, if it's nullptr then find the first StaticMesh
		if(USkeletalMeshComponent* SkelMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>(); SkelMeshComponent->GetSkeletalMeshAsset())
		{
			MeshComponentRef = SkelMeshComponent;
		}
		else if(UStaticMeshComponent* StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>(); StaticMeshComponent->GetStaticMesh())
		{
			MeshComponentRef = StaticMeshComponent;
		}
	}
}
