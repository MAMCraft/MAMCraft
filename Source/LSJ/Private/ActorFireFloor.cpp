#include "ActorFireFloor.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorFireFloor.h"

// Sets default values
AActorFireFloor::AActorFireFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorFireFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorFireFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorFireFloor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		//µ¨¸®°ÔÀÌÆ®
		//OtherActor->OnTakeAnyDamage();
	}
}

