// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyZombiePawn.h"
#include "Components/CapsuleComponent.h"
#include "AIControllerZombie.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemyZombiePawn::AEnemyZombiePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	skMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESHCOMPONENT"));
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMPONENT"));
	handAttackComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HANDATTACKCOMPONENT"));
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	statComponent = CreateDefaultSubobject<UStatComponent>(TEXT("STAT"));

	rootComp->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	rootCompScale = FVector(1.0f, 1.0f, 1.0f);
	rootComp->SetWorldScale3D(rootCompScale);

	RootComponent = rootComp;
	capsuleComponent->SetupAttachment(rootComp);
	skMeshComponent->SetupAttachment(capsuleComponent);

	capsuleComponent->SetCapsuleHalfHeight(90.0f);
	capsuleComponent->SetCapsuleRadius(22.0f);
	skMeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	FName rightHandSocketName = TEXT("J_R_Hand");
	handAttackComponent->SetupAttachment(skMeshComponent, rightHandSocketName);
	handAttackComponent->SetRelativeLocation(FVector(0.3f, 0.2f, 0.0f));
	handAttackComponent->SetRelativeScale3D(FVector(0.016f, 0.009f, 0.004f));
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


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
	handAttackComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyZombiePawn::OnOverlapBegin);
}
void AEnemyZombiePawn::AttackHitCheck()
{
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AEnemyZombiePawn::AttackHitEnd()
{
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AEnemyZombiePawn::Attack()
{
	animInstance = Cast<UAnimInstanceZombie>(skMeshComponent->GetAnimInstance());
	if (animInstance == nullptr)
	{
		return;
	}
	animInstance->PlayAttackMontage();
}
void AEnemyZombiePawn::OnAttackEnd()
{
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UBlackboardComponent* bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
	bc->SetValueAsBool(FName("IsAttacking"), false);
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

void AEnemyZombiePawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		FDamageEvent damageEvent;
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());
		// �������� �޴����� TakeDamage�� ȣ���Ѵ�
		OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
		UE_LOG(LogTemp, Display, TEXT("%s %d"), *OtherActor->GetName(), statComponent->GetAttackDamage());
	}
}

float AEnemyZombiePawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	statComponent->OnAttacked(damage);

	return damage;
}
