#include "Ability/Activation/BaseActivation.h"
#include "GameFramework/Character.h"

void UBaseActivation::InitActivationMechanism()
{
	InitAssignMeshComponentRef();
}

void UBaseActivation::InitAssignMeshComponentRef()
{
	const AActor* ParentAbility = Cast<AActor>(GetOuter());
	if(!ParentAbility || !ParentAbility->GetOwner())
	{
		return;
	}
	
	// Find the first non-null mesh component that we come across
	if(MeshType == EMeshType::InstigatorMesh)
	{
		const APawn* AbilityInstigator = ParentAbility->GetInstigator();
		if(!AbilityInstigator)
		{
			return;
		}
		
		// Check if Instigator is a char, if so then GetMesh(). Otherwise, find first skel mesh
		if(const ACharacter* CharRef = Cast<ACharacter>(AbilityInstigator))
		{
			MeshComponentRef = CharRef->GetMesh();
		} else
		{
			MeshComponentRef = AbilityInstigator->FindComponentByClass<USkeletalMeshComponent>();	
		}
	} else if (MeshType == EMeshType::AbilityMesh)
	{
		// Find the first SkeletalMesh, if it's nullptr then find the first StaticMesh
		if(USkeletalMeshComponent* SkelMeshComponent = ParentAbility->FindComponentByClass<USkeletalMeshComponent>(); SkelMeshComponent->GetSkeletalMeshAsset())
		{
			MeshComponentRef = SkelMeshComponent;
		}
		else if(UStaticMeshComponent* StaticMeshComponent = ParentAbility->FindComponentByClass<UStaticMeshComponent>(); StaticMeshComponent->GetStaticMesh())
		{
			MeshComponentRef = StaticMeshComponent;
		}
	}
}
