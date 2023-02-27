// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Taggable.h"
#include "CommonActor.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonActor : public AActor, public ITaggable
{
	GENERATED_BODY()

public:
	ACommonActor();
	
	////////////////////////////////
	/// ITaggable override
	////////////////////////////////
	UFUNCTION()
	virtual FGameplayTagContainer& GetTagContainer() override { return GameplayTagContainer; }
	virtual void HandleTagChanged(const FGameplayTag& ChangedTag, bool bAdded) override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "CUSTOM|CommonActor")
	void K2_HandleTagAdded(const FGameplayTag& AddedTag);
	UFUNCTION(BlueprintNativeEvent, Category = "CUSTOM|CommonActor")
	void K2_HandleTagRemoved(const FGameplayTag& RemovedTag);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_HandleMeshOutlining(bool bIsOutlining);
	UFUNCTION(BlueprintImplementableEvent)
	void K2_HandleInteraction(AActor* InstigatingActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;

private:
	
	void Internal_AddDefaultTagsToContainer();
};
