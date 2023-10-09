// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimationComponent.h"
#include "Components/ActorComponent.h"
#include "ThirdPersonAnimationComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UThirdPersonAnimationComponent : public UCharacterAnimationComponent
{
	GENERATED_BODY()

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	class UThirdPersonAnimInstance* ThirdPersonAnimInstance;
};
