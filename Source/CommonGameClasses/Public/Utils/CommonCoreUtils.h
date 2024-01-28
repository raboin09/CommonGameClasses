// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorComponent/QuestManagerComponent.h"
#include "Core/ActorTrackingSubsystem.h"
#include "Core/CommonGameInstance.h"
#include "Core/CommonGameMode.h"
#include "CommonCoreUtils.generated.h"

class UHealthComponent;
class ACommonPlayerCharacter;
class ACommonPlayerController;

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonCoreUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool IsObjectPlayerControlled(const UObject* Object);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UActorTrackingSubsystem* GetActorTrackingSubsystem(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static ACommonGameMode* GetCommonGameMode(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static ACommonPlayerCharacter* GetCommonPlayerCharacter(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static ACommonPlayerController* GetCommonPlayerController(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UInteractionComponent* GetHoveredInteractionComponentByPlayerController(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UHealthComponent* GetPlayerCharacterHealthComponent(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UHealthComponent* GetHealthComponentFromActor(UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UQuestManagerComponent* GetQuestManager(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UCommonGameInstance* GetCommonGameInstance(const UObject* ContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static TScriptInterface<ILevelLoadingManager> GetLevelLoadingManager(const UObject* ContextObject);

	template<typename M, typename I>
	static FORCEINLINE TScriptInterface<I> CreateInterfaceOfType(UObject* OuterObject)
	{
		UObject* TempInterfaceObj = NewObject<M>(OuterObject, M::StaticClass());
		TScriptInterface<I> TempInterface;
		TempInterface.SetObject(TempInterfaceObj);
		TempInterface.SetInterface(Cast<I>(TempInterfaceObj));
		return TempInterface;
	}
};
