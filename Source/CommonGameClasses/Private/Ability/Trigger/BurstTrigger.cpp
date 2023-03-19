// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Trigger/BurstTrigger.h"


// Sets default values
ABurstTrigger::ABurstTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABurstTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABurstTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

