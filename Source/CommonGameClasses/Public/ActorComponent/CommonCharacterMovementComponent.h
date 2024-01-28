// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterMovementComponent.h"
#include "CommonCharacterMovementComponent.generated.h"


class ACommonCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Variable", Sockets, "Character Movement (General Settings)", "ComponentTick", "Character Movement: Walking",
	"Character Movement: Jumping / Falling", "Character Movement: Gravity", "Character Movement: (Networking)", "Character Movement: Swimming", "Character Movement: Flying", "Character Movement: Custom Movement",
	"CharacterMovement (Rotation Settings)", "Character Movement: Physics Interaction", "Character Movement", "Character Movement: Avoidance", "Root Motion", "Character Movement: Nav Mesh Movement",
	"Nav Movement", "Velocity", "PlanarMovement", "MovementComponent", "Tags", "ComponentReplication", "Activation", "Cooking", "Events", "AssetUserData", "Replication", "Navigation"))
class COMMONGAMECLASSES_API UCommonCharacterMovementComponent : public UAlsCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void SetWalkSpeedRatio(float Ratio);
	
private:
	UPROPERTY()
	TWeakObjectPtr<ACommonCharacter> OwnerCharacter;
};
