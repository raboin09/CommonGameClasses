// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonInputUtils.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMECLASSES_API UCommonInputUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="COMMON|InputUtils", meta=(WorldContext="WorldContextObject"))
	static bool IsUsingGamepad(const UObject* WorldContextObject);
};
