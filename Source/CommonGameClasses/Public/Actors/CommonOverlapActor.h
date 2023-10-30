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
	void K2_HandleActivation();
	UFUNCTION(BlueprintCallable, Category="COMMON")
	void Activate();

	UFUNCTION(BlueprintImplementableEvent, Category="COMMON|Events")
	void K2_HandleDeactivation();
	UFUNCTION(BlueprintCallable, Category="COMMON")
	void Deactivate();
	
	UFUNCTION(BlueprintNativeEvent, Category="COMMON")
	UMeshComponent* GetMesh() const;
	virtual UMeshComponent* GetMesh_Implementation() const PURE_VIRTUAL(ACommonOverlapActor::GetMesh_Implementation, return nullptr;);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON")
	UShapeComponent* GetCollisionComponent() const;
	virtual UShapeComponent* GetCollisionComponent_Implementation() const  PURE_VIRTUAL(ACommonOverlapActor::GetCollisionComponent_Implementation, return nullptr;);
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON")
	FORCEINLINE bool IsActive() { return UGameplayTagComponent::ActorHasGameplayTag(this, CommonGameState::Active); }
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COMMON|Events")
	void K2_HandleOverlapEvent(AActor* OtherActor, const FHitResult& HitResult);
	virtual void K2_HandleOverlapEvent_Implementation(AActor* OtherActor, const FHitResult& HitResult);
	UFUNCTION(BlueprintNativeEvent, Category="COMMON|Events")
	void K2_HandleEndOverlapEvent(AActor* ExitingActor);
	virtual void K2_HandleEndOverlapEvent_Implementation(AActor* ExitingActor);
	
	virtual void HandleActorDeath();	
	UFUNCTION()
	virtual void ActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, Category = "CUSTOM|Overlap")
	TArray<FGameplayTag> BlockedOverlapTags;
	UPROPERTY(EditAnywhere, Category = "CUSTOM|Overlap")
	TArray<FGameplayTag> RequiredOverlapTags;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Overlap")
	bool bActivateOnStart = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUSTOM|Overlap")
	bool bDiesAfterOverlap;
	UPROPERTY(EditDefaultsOnly, Category = "CUSTOM|Overlap", meta = (EditCondition = "bDiesAfterOverlap", EditConditionHides))
	float DeathBuffer;

	UPROPERTY()
	TArray<AActor*> HitActors;
};
