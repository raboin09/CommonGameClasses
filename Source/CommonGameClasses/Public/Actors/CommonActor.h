// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonActor.generated.h"

struct FGameplayTag;
class UGameplayTagComponent;

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonActor : public AActor
{
	GENERATED_BODY()

public:
	ACommonActor();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM")
	TArray<FGameplayTag> DefaultGameplayTags;

private:
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;
};
