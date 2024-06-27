// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyZombie.h"
#include "EnemyZombieController.h"
// Sets default values
AEnemyZombie::AEnemyZombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skCardboard(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (skCardboard.Succeeded())
	{
		skMeshComponent->SetSkeletalMesh(skCardboard.Object);
	}
	skMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> zombieAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/LSJ/Animation/BPZombieAnim.BPZombieAnim_C'"));
	if (zombieAnim.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("zombieAnim.Succeeded()"));
		skMeshComponent->SetAnimInstanceClass(zombieAnim.Class);
	}
	else
		UE_LOG(LogTemp, Log, TEXT("zombieAnim.Fail"));

	AIControllerClass = AEnemyZombieController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyZombie::BeginPlay()
{
	Super::BeginPlay();

	

	if (localPatrolPoints.Num() == 0)
	{
		localPatrolPoints.Add({ 0, 0, 0 });
	}
	for (FVector localPatrolPoint : localPatrolPoints)
	{
		worldPatrolPoints.Add(localPatrolPoint * GetActorLocation());
	}
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

void AEnemyZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEnemyZombie::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
}

const FVector& AEnemyZombie::GetNextPatrolPoint()
{
	if (worldPatrolPoints.Num() > 0)
	{
		return worldPatrolPoints[FMath::RandRange(0, localPatrolPoints.Num() - 1)];
	}
	return worldPatrolPoints[0];
}

