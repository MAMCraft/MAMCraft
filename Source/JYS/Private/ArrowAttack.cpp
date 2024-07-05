// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowAttack.h"
#include "Components/BoxComponent.h"


// Sets default values
AArrowAttack::AArrowAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(37.5f, 12.5f, 50.f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);


	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(0.75f, 0.25f, 1));
	}
}

// Called when the game starts or when spawned
void AArrowAttack::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArrowAttack::DestroyArrow, 1.0f, true);
}

// Called every frame
void AArrowAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector P0 = GetActorLocation();
	FVector velocity = GetActorForwardVector() * Speed;
	float t = DeltaTime;
	FVector NewLocation = P0 + velocity * t;
	SetActorLocation(NewLocation);
}

void AArrowAttack::DestroyArrow()
{
	Destroy();
}



