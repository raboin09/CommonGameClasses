// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Trigger/MeleeTrigger.h"


// Sets default values
AMeleeTrigger::AMeleeTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMeleeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

