// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/CommonTypes.h"
#include "GameplayTagComponent.h"
#include "API/Mountable.h"
#include "GameFramework/Character.h"
#include "CommonCharacter.generated.h"

UCLASS(Abstract, Blueprintable)
class COMMONGAMECLASSES_API ACommonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACommonCharacter();

	FORCEINLINE TScriptInterface<IMountable> GetCurrentMount() const { return CurrentMount; }

protected:

	virtual void BeginPlay() override;
	
	////////////////////////////////
	/// Common Events
	////////////////////////////////
	UFUNCTION(BlueprintNativeEvent, Category = "CUSTOM|Character")
	void K2_OnDeath();
	virtual void K2_OnDeath_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CUSTOM|Defaults")
	TArray<FGameplayTag> DefaultGameplayTags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CUSTOM|Defaults")
	EAffiliation AbsoluteAffiliation;
	
	UPROPERTY()
	UGameplayTagComponent* GameplayTagComponent;
	UPROPERTY()
	TScriptInterface<IMountable> CurrentMount;
};
