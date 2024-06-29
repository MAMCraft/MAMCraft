// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyZombiePawn.h"
#include "Components/CapsuleComponent.h"
#include "AIControllerZombie.h"

// Sets default values
AEnemyZombiePawn::AEnemyZombiePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	skMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESHCOMPONENT"));
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMPONENT"));
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));

	rootComp->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	rootCompScale = FVector(1.0f, 1.0f, 1.0f);
	rootComp->SetWorldScale3D(rootCompScale);



	RootComponent = rootComp;
	capsuleComponent->SetupAttachment(rootComp);
	skMeshComponent->SetupAttachment(capsuleComponent);

	capsuleComponent->SetCapsuleHalfHeight(90.0f);
	capsuleComponent->SetCapsuleRadius(22.0f);
	skMeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skCardboard(TEXT("/Script/Engine.SkeletalMesh'/Game/LSJ/Resource/Zombie/Skeleton/zombie.zombie'"));
	if (skCardboard.Succeeded())
	{
		skMeshComponent->SetSkeletalMesh(skCardboard.Object);
	}
	skMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> zombieAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/LSJ/Resource/Zombie/BP_AnimInstanceZombie.BP_AnimInstanceZombie_C'"));
	if (zombieAnim.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("zombieAnim.Succeeded()"));
		skMeshComponent->SetAnimInstanceClass(zombieAnim.Class);
	}
	else
		UE_LOG(LogTemp, Log, TEXT("zombieAnim.Fail"));

	AIControllerClass = AAIControllerZombie::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyZombiePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyZombiePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyZombiePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

