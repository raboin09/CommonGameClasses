// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/GameplayTagComponent.h"
#include "CommonActor.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonActor : public AActor
{
	GENERATED_BODY()

public:
	ACommonActor();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;

private:
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;
};
