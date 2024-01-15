// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagComponent.h"
#include "API/Mountable.h"
#include "Components/ActorComponent.h"
#include "Types/CommonTagTypes.h"
#include "MountManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMMONGAMECLASSES_API UMountManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMountManagerComponent();
	
protected:
	UPROPERTY()
	TScriptInterface<IMountable> CurrentMount;

public:
	FORCEINLINE void AssignNewMountable(UObject* InMountableObject, const FHitResult& InHitResult);
	FORCEINLINE TScriptInterface<IMountable> GetCurrentMount() const { return CurrentMount; }
	FORCEINLINE bool IsMounted() const { return CurrentMount != nullptr; }
	FORCEINLINE bool CanGetInCover() const { return !UGameplayTagComponent::ActorHasGameplayTag(GetOwner(), CommonGameState::CannotMount); }
};
