// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyZombie.h"

// Sets default values
AEnermyZombie::AEnermyZombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnermyZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnermyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnermyZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

