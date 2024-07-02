// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBlazePawn.h"
#include "AIControllerBlaze.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ActorBlazeBullet.h"
// Sets default values
AEnemyBlazePawn::AEnemyBlazePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	skMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESHCOMPONENT"));
	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BOXCOMPONENT"));
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	boxComponent->SetRelativeScale3D_Direct(FVector(1.f, 1.f, 2.f));
	statComponent = CreateDefaultSubobject<UStatComponent>(TEXT("STAT"));

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
	static ConstructorHelpers::FClassFinder<AActorBlazeBullet> blazeAnim1(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Blueprints/BP_BlazeBullet.BP_BlazeBullet_C'"));
	if (blazeAnim1.Succeeded())
	{
		bullet =(blazeAnim1.Class);
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
	animInstance = Cast<UAnimInstanceBlaze>(skMeshComponent->GetAnimInstance());
	animInstance->BulletFire.AddUObject(this, &AEnemyBlazePawn::Fire);
	statComponent->SetLevel(FName("Blaze"));
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

void AEnemyBlazePawn::AttackHitCheck()
{
	attackRangeindex = 0;
	// bullet 발사
	Fire();
}

void AEnemyBlazePawn::AttackHitEnd()
{
}
//안쓰는 함수
void AEnemyBlazePawn::Attack()
{
	if (animInstance == nullptr)
	{
		UBlackboardComponent* bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
		bc->SetValueAsBool(FName("IsAttacking"), false);
		animInstance = Cast<UAnimInstanceBlaze>(skMeshComponent->GetAnimInstance());
		return;
	}
	movement->MaxSpeed = 0;
	animInstance->PlayAttackMontage();
}
void AEnemyBlazePawn::Fire()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	AActorBlazeBullet* bulletInstance = Cast<AActorBlazeBullet>(GetWorld()->SpawnActor<AActorBlazeBullet>(bullet, GetActorLocation(), GetActorRotation(), SpawnParams));
	bulletInstance->SetAttacklocation(attackLocation[attackRangeindex]);
	bulletInstance->SetDamage(statComponent->GetAttackDamage(), (statComponent->GetAttackDamage()/10));

	attackRangeindex++;
}
void AEnemyBlazePawn::OnAttackEnd()
{
	UBlackboardComponent* bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
	bc->SetValueAsBool(FName("IsAttacking"), false);
	bc->SetValueAsBool(FName("WasRunAway"), false);
	movement->MaxSpeed = 300.f;
	
}

void AEnemyBlazePawn::Attack(TArray<FVector>& location)
{
	attackLocation = location;
	if (animInstance == nullptr)
	{
		UBlackboardComponent* bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
		bc->SetValueAsBool(FName("IsAttacking"), false);
		animInstance = Cast<UAnimInstanceBlaze>(skMeshComponent->GetAnimInstance());
		return;
	}
	movement->MaxSpeed = 0;
	animInstance->PlayAttackMontage();
}

