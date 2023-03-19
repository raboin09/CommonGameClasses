// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Activation/ProjectileActivation.h"


// Sets default values
AProjectileActivation::AProjectileActivation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileActivation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileActivation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

