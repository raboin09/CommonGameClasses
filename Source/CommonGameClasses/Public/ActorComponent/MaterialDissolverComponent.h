// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MaterialDissolverComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UMaterialDissolverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMaterialDissolverComponent();
	
	void InitDissolveableMesh(UMeshComponent* InMesh);
	void StartAppearTimeline();
	void StartDissolveTimeline();
	
protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category="Genestealer")
	float StartingDissolveValue = 0.f;
	UPROPERTY(EditDefaultsOnly, Category="Genestealer")
	FName DissolveParameterName;
	UPROPERTY(EditDefaultsOnly, Category="Genestealer")
	FName ColorParameterName;
	UPROPERTY(EditAnywhere, Category="Genestealer")
	UCurveFloat* DissolveCurveFloat;

private:
	UFUNCTION()
	void Internal_TimelineDissolveEnd();
	UFUNCTION()
	void Internal_TimelineDissolveUpdate(float Value);
	UFUNCTION()
	void InitDissolveTimeline();
	
	UPROPERTY()
	FTimeline DissolveTimeline;
	UPROPERTY()
	UMeshComponent* MeshComponent;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DissolveDynamicMaterialInstances;
};
