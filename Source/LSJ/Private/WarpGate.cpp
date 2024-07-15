// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpGate.h"

// Sets default values
AWarpGate::AWarpGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	mesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/LSJ/Resource/MapResource/Potal/SM_MERGED_StaticMeshActor.SM_MERGED_StaticMeshActor'"));
	if (sphereMesh.Succeeded())
	{
		mesh->SetStaticMesh(sphereMesh.Object);
	}
}

// Called when the game starts or when spawned
void AWarpGate::BeginPlay()
{
	Super::BeginPlay();
	mesh->OnComponentBeginOverlap.AddDynamic(this, &AWarpGate::OverlapBegin);
}

void AWarpGate::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
		if(state==0)
			UGameplayStatics::OpenLevel(this, TEXT("Hell"));
		else if (state == 1)
			return;
}

// Called every frame
void AWarpGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

