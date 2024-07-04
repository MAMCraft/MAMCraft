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

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMPONENT"));
	capsuleComponent->SetCapsuleHalfHeight(90.0f);
	capsuleComponent->SetCapsuleRadius(22.0f);
	capsuleComponent->SetupAttachment(GetRootComponent());
	//rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	skMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESHCOMPONENT"));
	skMeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	skMeshComponent->SetupAttachment(capsuleComponent);
	handAttackComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HANDATTACKCOMPONENT"));
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	statComponent = CreateDefaultSubobject<UStatComponent>(TEXT("STAT"));

	//RootComponent = capsuleComponent;
	//capsuleComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	
	//rootCompScale = FVector(1.0f, 1.0f, 1.0f);
	//capsuleComponent->SetWorldScale3D(rootCompScale);
	
	
	//capsuleComponent->SetupAttachment(skMeshComponent);
	


	
	FName rightHandSocketName = TEXT("J_R_Hand");
	handAttackComponent->SetupAttachment(skMeshComponent, rightHandSocketName);
	handAttackComponent->SetRelativeLocation(FVector(0.3f, 0.2f, 0.0f));
	handAttackComponent->SetRelativeScale3D(FVector(0.02f, 0.01f, 0.006f));
	//	handAttackComponent->SetRelativeScale3D(FVector(0.016f, 0.009f, 0.004f));
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
	bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
	animInstance = Cast<UAnimInstanceZombie>(skMeshComponent->GetAnimInstance());
	handAttackComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyZombiePawn::OnOverlapBegin);
	capsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyZombiePawn::OnBeginOverlap);
	animInstance->hitEnd.AddUObject(this, &AEnemyZombiePawn::HitEnd);
	groundZValue = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation().Z;
}
void AEnemyZombiePawn::Attack(TArray<FVector>& location)
{
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
	if (bHit)
	{
		OnAttackEnd();
		return;
	}
	
	if (animInstance == nullptr)
	{
		//bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
		bc->SetValueAsBool(FName("IsAttacking"), false);
		animInstance = Cast<UAnimInstanceZombie>(skMeshComponent->GetAnimInstance());
	}
	animInstance->PlayAttackMontage();
}
void AEnemyZombiePawn::OnAttackEnd()
{
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
	bc->SetValueAsBool(FName("IsAttacking"), false);
}
// Called every frame
void AEnemyZombiePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	pawnLocation = GetActorLocation();
	if (bHit)
	{
		currentTime += DeltaTime;

		SetActorLocation(GetActorLocation() + direction * FMath::Lerp(5.0f,10.f,0.5));

		//if (currentTime<hitKnockbackTime)
		//{
		//	FVector velocity = {
		//		FMath::Lerp(1, outVelocity.X, 0.8),
		//		FMath::Lerp(1, outVelocity.Y, 0.8),
		//		FMath::Lerp(1, outVelocity.Z, 0.8)
		//	};
		//	pawnLocation += velocity;
		//	//SetActorLocation(GetActorLocation() + velocity);
		//}
		//UE_LOG(LogTemp, Warning, TEXT("ComponentSpace %f %f %f"), skMeshComponent->GetBoneLocation(FName("J_Root"), EBoneSpaces::ComponentSpace).X,
		//	skMeshComponent->GetBoneLocation(FName("J_Root"), EBoneSpaces::ComponentSpace).Y,
		//	skMeshComponent->GetBoneLocation(FName("J_Root"), EBoneSpaces::ComponentSpace).Z);
		//UE_LOG(LogTemp, Warning, TEXT("WorldSpace %f %f %f"), skMeshComponent->GetBoneLocation(FName("J_Root"), EBoneSpaces::WorldSpace).X,
		//	skMeshComponent->GetBoneLocation(FName("J_Root"), EBoneSpaces::WorldSpace).Y,
		//	skMeshComponent->GetBoneLocation(FName("J_Root"), EBoneSpaces::WorldSpace).Z);
		//FVector meshMoveLocation = skMeshComponent->GetBoneLocation(FName("J_Root"), EBoneSpaces::ComponentSpace)*0.1f;
		//meshMoveLocation.X = GetActorLocation().X;
		//meshMoveLocation.Z = GetActorLocation().Z + 62.5f;
		//pawnLocation.Z += 62.5f;
		//SetActorLocation(pawnLocation);
		if (currentTime > hitTime)
		{
			currentTime = 0;
			bHit = false;
		}
	}

	//if (pawnLocation.Z > groundZValue + 0.1f)
	//{
	//	//FVector currentLocation = GetActorLocation();
	//	//currentLocation.Z -= 1.5f;
	//	pawnLocation.Z -= (1.f + (pawnLocation.Z - groundZValue + 10.0f) * 0.07f);
	//	//SetActorLocation(currentLocation);
	//	UE_LOG(LogTemp, Log, TEXT("high currentLocation.Z %f"), pawnLocation.Z);
	//}
	//else if (pawnLocation.Z < groundZValue - 0.1f)
	//{
	//	//FVector currentLocation = GetActorLocation();
	//	pawnLocation.Z += 2.f;
	//	//SetActorLocation(currentLocation);
	//	UE_LOG(LogTemp, Log, TEXT("low currentLocation.Z %f"), pawnLocation.Z);
	//}
	//SetActorLocation(pawnLocation);
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
		// 데미지를 받는쪽의 TakeDamage를 호출한다
		OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
	}
}

float AEnemyZombiePawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	statComponent->OnAttacked(damage);

	//넉백 추가 예정
	/*FVector direction = DamageCauser->GetActorLocation() - GetActorLocation();
	FVector directionNor = direction.GetSafeNormal();
	float power = 100.0f;
	capsuleComponent->AddForce(directionNor * power);*/
	
	if (statComponent->GetHp() <= 0)
	{

		//애니메이션 Die
		//넉백 추가 예정
		//collision off
	}
	else
	{
		//애니메이션 HIt
		if (animInstance == nullptr)
		{
			animInstance = Cast<UAnimInstanceZombie>(skMeshComponent->GetAnimInstance());
			return damage;
		}
		//animInstance->PlayHitMontage();
	}
	return damage;
}

void AEnemyZombiePawn::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent DamageEvent;
	TakeDamage(1, DamageEvent, nullptr, this);
	Hit(OtherActor);
}
//안쓸예정
void AEnemyZombiePawn::HitEnd()
{
	//meshMoveLocation.Z = groundZValue;
	//SetActorLocation(meshMoveLocation);
	//UE_LOG(LogTemp, Error, TEXT("HitEnd"));
}

void AEnemyZombiePawn::Hit(AActor* damageCauser)
{
	if (bc->GetValueAsBool(FName("IsAttacking")))
		return;
	if (currentTime > 0)
		return;
	bHit = true;
	// knock back
	//FVector startLoc = GetActorLocation();      // 발사 지점
	//FVector targetLoc = GetActorLocation()+(GetActorLocation() - damageCauser->GetActorLocation()).GetSafeNormal2D() * FVector(0.5f,0.5f,2.f) * 1.f;  // 타겟 지점.
	//float arcValue = 0.5f;                       // ArcParam (0.0-1.0)
	//outVelocity = FVector::ZeroVector;   // 결과 Velocity
	//if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	//{
	//	//FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
	//	//predictParams.DrawDebugTime = 15.0f;     //디버그 라인 보여지는 시간 (초)
	//	//predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
	//	//predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
	//	//FPredictProjectilePathResult result;
	//	//UGameplayStatics::PredictProjectilePath(this, predictParams, result);
	//	//skMeshComponent->AddImpulse(outVelocity * 100); // objectToSend는 발사체 * 질량 해줘야되는거 같다.
	//}
	//Hit Montage

	if (animInstance == nullptr)
	{
		animInstance = Cast<UAnimInstanceZombie>(skMeshComponent->GetAnimInstance());
	}
	animInstance->PlayHitMontage();
	//pawnLocation.Z += 62.5f;
	//SetActorLocation(pawnLocation);
	direction = (GetActorLocation() - damageCauser->GetActorLocation()).GetSafeNormal();
	direction.Z = 0.f;
	//UE_LOG(LogTemp, Error, TEXT("GetSafeNormal %f %f %f"), (GetActorLocation()-damageCauser->GetActorLocation()).GetSafeNormal().X,
	//	(GetActorLocation() - damageCauser->GetActorLocation()).GetSafeNormal().Y,
	//	(GetActorLocation() - damageCauser->GetActorLocation()).GetSafeNormal().Z);
	//UE_LOG(LogTemp, Error, TEXT("HitEnd"));
	//capsuleComponent->AddForce(GetActorLocation() - damageCauser->GetActorLocation().GetSafeNormal() * 500.f);
}