// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Activation/MeleeOverlapActivation.h"


// Sets default values
AMeleeOverlapActivation::AMeleeOverlapActivation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMeleeOverlapActivation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeOverlapActivation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

