// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBlazePawn.h"
#include "AIControllerBlaze.h"

// Sets default values
AEnemyBlazePawn::AEnemyBlazePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	//rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	skMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESHCOMPONENT"));
	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BOXCOMPONENT"));
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	boxComponent->SetRelativeScale3D_Direct(FVector(1.f, 1.f, 2.f));
	//boxComponent->SetRelativeScale3D();

	boxComponent->SetupAttachment(GetRootComponent());
	skMeshComponent->SetupAttachment(boxComponent);
	//skMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 0.5f));
	skMeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -35.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skCardboard(TEXT("/Script/Engine.SkeletalMesh'/Game/LSJ/Resource/Blaze/Mesh/blaze.blaze'"));
	if (skCardboard.Succeeded())
	{
		skMeshComponent->SetSkeletalMesh(skCardboard.Object);
	}
	skMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> blazeAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/LSJ/Resource/Blaze/BP_AnimInstanceBlaze.BP_AnimInstanceBlaze_C'"));
	if (blazeAnim.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("blazeAnim.Succeeded()"));
		skMeshComponent->SetAnimInstanceClass(blazeAnim.Class);
	}

	AIControllerClass = AAIControllerBlaze::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

UBehaviorTree* AEnemyBlazePawn::GetBehaviorTree() const
{
	return bTree;
}

// Called when the game starts or when spawned
void AEnemyBlazePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBlazePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBlazePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

