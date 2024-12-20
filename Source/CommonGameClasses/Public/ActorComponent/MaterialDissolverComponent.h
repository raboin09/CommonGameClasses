﻿// Fill out your copyright notice in the Description page of Project Settings.

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

	UFUNCTION(BlueprintCallable, Category="COMMON|Dissolver")
	void InitDissolveableMesh(UMeshComponent* InMesh);
	UFUNCTION(BlueprintCallable, Category="COMMON|Dissolver")
	void StartAppearTimeline();
	UFUNCTION(BlueprintCallable, Category="COMMON|Dissolver")
	void StartDissolveTimeline();
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category="CUSTOM")
	float StartingDissolveValue = 0.f;
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FName DissolveParameterName = "Dissolve Amount";
	UPROPERTY(EditDefaultsOnly, Category="CUSTOM")
	FName ColorParameterName = "Color";
	UPROPERTY(EditAnywhere, Category="CUSTOM")
	float DissolveDuration = 1.f;

private:  
	UFUNCTION()
	void Internal_TimelineDissolveEnd();
	UFUNCTION()
	void Internal_TimelineDissolveUpdate();
	UFUNCTION()
	void Internal_SetDissolveMeshValues(float Value);
	UFUNCTION()
	void InitDissolveTimeline();
	
	UPROPERTY()
	FTimeline DissolveTimeline;
	UPROPERTY()
	TWeakObjectPtr<UMeshComponent> MeshComponent;
	UPROPERTY()
	TArray<TSoftObjectPtr<UMaterialInstanceDynamic>> DissolveDynamicMaterialInstances;
};
 