// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowItem.h"

// Sets default values
AArrowItem::AArrowItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	this->SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	boxComp->SetBoxExtent(FVector(20.0f));
	boxComp->SetRelativeScale3D(FVector(1.0f));

	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionProfileName(TEXT("Arrowitem"));
}

// Called when the game starts or when spawned
void AArrowItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrowItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

