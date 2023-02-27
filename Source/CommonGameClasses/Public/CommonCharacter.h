// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.h"
#include "Taggable.h"
#include "GameFramework/Character.h"
#include "CommonCharacter.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonCharacter : public ACharacter, public ITaggable
{
	GENERATED_BODY()

public:
	ACommonCharacter();

	////////////////////////////////
	/// ITaggable override
	////////////////////////////////
	UFUNCTION()
	virtual FGameplayTagContainer& GetTagContainer() override { return GameplayTagContainer; }
	virtual void HandleTagChanged(const FGameplayTag& ChangedTag, bool bAdded) override;

protected:
	////////////////////////////////
	/// Common Events
	////////////////////////////////
	UFUNCTION(BlueprintNativeEvent, Category = "CUSTOM|Character")
	void K2_OnDeath();
	UFUNCTION(BlueprintNativeEvent, Category = "CUSTOM|Character")
	void K2_HandleTagAdded(const FGameplayTag& AddedTag);
	UFUNCTION(BlueprintNativeEvent, Category = "CUSTOM|Character")
	void K2_HandleTagRemoved(const FGameplayTag& RemovedTag);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CUSTOM|Defaults")
	EAffiliation AbsoluteAffiliation;
};
