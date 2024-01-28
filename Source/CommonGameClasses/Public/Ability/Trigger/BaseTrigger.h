 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "API/Ability/TriggerMechanism.h"
#include "UObject/Object.h"
#include "BaseTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType)
class COMMONGAMECLASSES_API UBaseTrigger : public UObject, public ITriggerMechanism
{
	GENERATED_BODY()

public:
	// Handles initial trigger press conditions (e.g. can this even start the press trigger logic). If it can, then
	// call HandleSuccessfulTriggerPressed. Otherwise, return early.
	virtual void PressTrigger() override;
	// Handles initial trigger release event and call HandleTriggerReleased
	virtual void ReleaseTrigger() override;
	virtual bool ShouldRetriggerAbilityAfterCooldown() const override PURE_VIRTUAL(UBaseTrigger::ShouldRetriggerAbilityAfterCooldown, return false; )

protected:
	virtual void HandleTriggerReleased() PURE_VIRTUAL(UBaseTrigger::HandleTriggerReleased, )
	virtual void HandleSuccessfulTriggerPressed() PURE_VIRTUAL(UBaseTrigger::HandleSuccessfulTriggerPressed, )
	
	virtual bool CanExecuteTriggerEvent() const;

	// What tags does the owning ability (e.g. a Shotgun) REQUIRE to fire successfully
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TArray<FGameplayTag> AbilityRequiredTags;
	// What tags BLOCK the owning ability (e.g. a Shotgun) from firing successfully
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TArray<FGameplayTag> AbilityBlockedTags;
	// What tags does the instigator (e.g. the player's ACharacter) REQUIRE to fire successfully
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TArray<FGameplayTag> InstigatorRequiredTags;
	// What tags BLOCK the instigator (e.g. the player's ACharacter) from firing successfully
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TArray<FGameplayTag> InstigatorBlockedTags;
};