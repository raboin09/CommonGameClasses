// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Cost/HealthCost.h"


// Sets default values
AHealthCost::AHealthCost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHealthCost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthCost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

