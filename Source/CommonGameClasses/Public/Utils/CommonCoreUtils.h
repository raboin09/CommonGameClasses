// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonCoreUtils.generated.h"

class UInteractionComponent;
enum class ECameraType : uint8;
class UActorTrackingSubsystem;
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static bool IsObjectPlayerControlled(const UObject* Object);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils", meta=(WorldContext="WorldContextObject"))
	static UActorTrackingSubsystem* GetActorTrackingSubsystem(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils", meta=(WorldContext="WorldContextObject"))
	static ACommonPlayerCharacter* GetCommonPlayerCharacter(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils", meta=(WorldContext="WorldContextObject"))
	static ACommonPlayerController* GetCommonPlayerController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils", meta=(WorldContext="WorldContextObject"))
	static ECameraType GetCurrentCameraType(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils", meta=(WorldContext="WorldContextObject"))
	static UInteractionComponent* GetHoveredInteractionComponentByPlayerController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils", meta=(WorldContext="WorldContextObject"))
	static AActor* GetHoveredInteractionActorByPlayerController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils", meta=(WorldContext="WorldContextObject"))
	static UHealthComponent* GetPlayerCharacterHealthComponent(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|CoreUtils")
	static UHealthComponent* GetHealthComponentFromActor(AActor* PotentialActor);

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
