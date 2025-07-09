// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActor.h"
#include "ActorComponent/GameplayTagComponent.h"
#include "Types/CommonTagTypes.h"
#include "CommonOverlapActor.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMMONGAMECLASSES_API ACommonOverlapActor : public ACommonActor
{
	GENERATED_BODY()

public:
	ACommonOverlapActor();

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_OnStartDetectingOverlaps();
	UFUNCTION(BlueprintCallable, Category="COMMON")
	void StartDetectingOverlaps();

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void BPI_OnStopDetectingOverlaps();
	UFUNCTION(BlueprintCallable, Category="COMMON")
	void StopDetectingOverlaps();
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON")
	UMeshComponent* BPN_GetMesh() const;
	virtual UMeshComponent* BPN_GetMesh_Implementation() const PURE_VIRTUAL(ACommonOverlapActor::BPN_GetMesh_Implementation, return nullptr;);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON")
	UShapeComponent* BPN_GetCollisionComponent() const;
	virtual UShapeComponent* BPN_GetCollisionComponent_Implementation() const  PURE_VIRTUAL(ACommonOverlapActor::BPN_GetCollisionComponent_Implementation, return nullptr;);
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
	/**
	 * Handles the start of a valid overlap event with another actor.
	 *
	 * This function is triggered when an overlapping actor is determined
	 * to meet the necessary conditions for a valid overlap interaction.
	 * It allows custom behavior to be defined when a valid overlap begins.
	 *
	 * @param OtherActor The actor that is beginning the overlap.
	 * @param HitResult Information about the component hit during the overlap event.
	 */

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Overlap")
	void BPI_OnValidActorBeginOverlap(AActor* OtherActor, const FHitResult& HitResult);
	/**
	 * Handles the end of a valid overlap event with another actor.
	 *
	 * This function is triggered when an overlapping actor is determined
	 * to no longer satisfy the conditions for a valid overlap interaction.
	 * It allows custom behavior to be defined when a valid overlap ends.
	 *
	 * @param ExitingActor The actor that is ending the overlap.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Overlap")
	void BPI_OnValidActorEndOverlap(AActor* ExitingActor);

	/**
	 * Determines whether the specified actor is eligible to trigger an overlap event.
	 *
	 * This function is used to evaluate if the provided actor meets the necessary
	 * conditions to be considered for triggering an overlap. It allows for custom logic
	 * to define the criteria for valid overlap interactions.
	 *
	 * @param OtherActor The actor being evaluated for overlap eligibility.
	 * @return True if the actor can trigger the overlap event, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Overlap")
	bool BPN_CanActorTriggerOverlap(AActor* OtherActor);
	
	virtual void HandleValidActorBeginOverlap(AActor* OtherActor, const FHitResult& HitResult);
	virtual void HandleValidActorEndOverlap(AActor* ExitingActor);
	
	virtual void HandleActorDeath();	
	UFUNCTION()
	virtual void ActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * Specifies the set of gameplay tags that prevent a valid overlap from occurring.
	 *
	 * This array defines tags that, if present on an overlapping actor, will block the overlap
	 * event from being recognized as valid. Any actor possessing one or more of the tags in this
	 * list will be excluded from triggering overlap-related logic. This allows for detailed control
	 * over entities that are permitted or excluded from overlap interactions.
	 */
	UPROPERTY(EditAnywhere, Category = "COMMON|Overlap")
	TArray<FGameplayTag> BlockedOverlapTags;
	/**
	 * Specifies the set of gameplay tags that an overlapping actor must possess to trigger a valid overlap.
	 *
	 * This array defines the required tags that an actor must match for the overlap event to be considered.
	 * Overlapping actors must have all the tags specified in this list for the event to proceed. If the overlapping
	 * actor lacks any of the required tags, the overlap will not be regarded as valid. This allows for fine-grained
	 * control over overlap behavior based on specified gameplay tag criteria.
	 */
	UPROPERTY(EditAnywhere, Category = "COMMON|Overlap")
	TArray<FGameplayTag> RequiredOverlapTags;
	/**
	 * Determines whether the actor should automatically detect overlaps when gameplay begins.
	 *
	 * When set to true, the actor will start detecting overlaps during the BeginPlay phase.
	 * If set to false, overlap detection will remain disabled until explicitly initiated
	 * through other means, such as invoking `StartDetectingOverlaps`. This setting enables
	 * or disables the initial overlap detection behavior at the start of the game.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Overlap")
	bool bDetectOverlapsOnBeginPlay = false;
	/**
	 * Indicates whether the actor should be destroyed after an overlap event.
	 *
	 * When set to true, the actor will be destroyed following a valid overlap event,
	 * according to the time duration specified by `DeathBuffer`. If set to false, the
	 * actor will not be automatically destroyed upon overlap. This behavior applies to
	 * interactions involving valid actors that match any specified overlap conditions.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COMMON|Overlap")
	bool bDiesAfterOverlap;
	/**
	 * Time buffer before the actor is destroyed after an overlap event.
	 *
	 * This variable determines how long the actor will remain in the world after
	 * a valid overlap has occurred, provided `bDiesAfterOverlap` is set to true.
	 * If `bDiesAfterOverlap` is false, this value is ignored and the actor will not
	 * be destroyed automatically upon overlap.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "COMMON|Overlap", meta = (EditCondition = "bDiesAfterOverlap", EditConditionHides))
	float DeathBuffer;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> HitActors;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON")
	FORCEINLINE bool IsDetectingOverlaps() { return UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Active); }
};
