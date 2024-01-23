// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CommonGameMode.h"

#include "Utils/CommonWorldUtils.h"

void ACommonGameMode::BeginPlay()
{
	UCommonWorldUtils::CurrentWorld = GetWorld();
	Super::BeginPlay();	
}
