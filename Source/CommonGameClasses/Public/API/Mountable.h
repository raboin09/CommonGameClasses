﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Mountable.generated.h"

class ACommonCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UMountable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMECLASSES_API IMountable
{
	GENERATED_BODY()

public:
	virtual void OccupyMount(ACommonCharacter* InActor, const FVector& InTargetCoverLocation, const FVector& InHitNormal) PURE_VIRTUAL(IMountable::OccupyMount, )
	virtual void VacateMount(ACommonCharacter* InActor) PURE_VIRTUAL(IMountable::VacateMount, )
	virtual void StartMountedAim() PURE_VIRTUAL(IMountable::StartMountedAim, )
	virtual void StopMountedAim() PURE_VIRTUAL(IMountable::StopMountedAim, )
	virtual void StartMountedFire() PURE_VIRTUAL(IMountable::StartMountedFire, )
	virtual void StopMountedFire() PURE_VIRTUAL(IMountable::StopMountedFire, )
};
