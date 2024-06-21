// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyZombie.h"

// Sets default values
AEnemyZombie::AEnemyZombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	skMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESHCOMPONENT"));
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMPONENT"));
	
	rootComp->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	rootCompScale = FVector(10.0f, 10.0f, 1.0f);
	rootComp->SetWorldScale3D(rootCompScale);



	RootComponent = rootComp;
	capsuleComponent->SetupAttachment(rootComp);
	skMeshComponent->SetupAttachment(capsuleComponent);

	capsuleComponent->SetCapsuleHalfHeight(90.0f);
	capsuleComponent->SetCapsuleRadius(22.0f);
	skMeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skCardboard(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (skCardboard.Succeeded())
	{
		skMeshComponent->SetSkeletalMesh(skCardboard.Object);
	}
}

// Called when the game starts or when spawned
void AEnemyZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

