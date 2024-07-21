// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyZombiePawn.h"
#include "Components/CapsuleComponent.h"
#include "AIControllerZombie.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/DamageEvents.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/WidgetComponent.h"
#include "UIDamageComponent.h"

// Sets default values
AEnemyZombiePawn::AEnemyZombiePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMPONENT"));
	capsuleComponent->SetCapsuleHalfHeight(90.0f);
	capsuleComponent->SetCapsuleRadius(50.0f);
	capsuleComponent->SetupAttachment(RootComponent);
	//rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	skMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESHCOMPONENT"));
	skMeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	skMeshComponent->SetupAttachment(capsuleComponent);
	handAttackComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HANDATTACKCOMPONENT"));
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));

	statComponent = CreateDefaultSubobject<UStatComponent>(TEXT("STAT"));
	


	
	FName rightHandSocketName = TEXT("J_R_Hand");
	handAttackComponent->SetupAttachment(skMeshComponent, rightHandSocketName);
	handAttackComponent->SetRelativeLocation(FVector(0.3f, 0.2f, 0.0f));
	handAttackComponent->SetRelativeScale3D(FVector(0.02f, 0.01f, 0.006f));
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
		skMeshComponent->SetAnimInstanceClass(zombieAnim.Class);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> hitMaterialFinder(TEXT("/Script/Engine.Material'/Game/LSJ/Resource/Zombie/Skeleton/MI_ZombieHit.MI_ZombieHit'"));
	if (hitMaterialFinder.Succeeded())
	{
		hitMaterial = hitMaterialFinder.Object;
	}
	//hpbar
	hpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	hpBarWidgetComponent->SetupAttachment(skMeshComponent);
	hpBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 180));
	hpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LSJ/Blueprints/WBP_HP.WBP_HP_C'"));
	if (UI_HUD.Succeeded())
	{
		hpBarWidgetComponent->SetWidgetClass(UI_HUD.Class);
		hpBarWidgetComponent->SetDrawSize(FVector2D(150, 50));
	}

	

	//DamageUI
	uiDamageComponent = CreateDefaultSubobject<UUIDamageComponent>(TEXT("UIDAMAGECOMPONENT"));
	//sound
	SoundDie = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LSJ/Sound/Zombie_Death.Zombie_Death'"));
	SoundAppear = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LSJ/Sound/zombie-_sound_effect.zombie-_sound_effect'"));
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
	//기준 바닥 Z값
	groundZValue = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation().Z;


	hpBarWidget = Cast<UHpBarWidget>(hpBarWidgetComponent->GetUserWidgetObject());
	if (nullptr != hpBarWidget)
	{
		hpBarWidget->BindCharacterStat(statComponent);
	}

}

void AEnemyZombiePawn::OnUnPossess()
{
	Super::UnPossessed();
	GetWorldTimerManager().ClearTimer(damageBlinkTimerHandle);
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
	if (animInstance == nullptr)
	{
		bc->SetValueAsBool(FName("IsAttacking"), false);
		animInstance = Cast<UAnimInstanceZombie>(skMeshComponent->GetAnimInstance());
	}
	animInstance->PlayAttackMontage();
}
void AEnemyZombiePawn::OnAttackEnd()
{
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bc->SetValueAsBool(FName("IsAttacking"), false);
}
// Called every frame
void AEnemyZombiePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	pawnLocation = GetActorLocation();


	if (bDie)
	{
		dieCurrentTime += DeltaTime;
		if (dieCurrentTime > dieDestroyTime)
		{
			Destroy();
		}
	}
}

// Called to bind functionality to input
void AEnemyZombiePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyZombiePawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		FDamageEvent damageEvent;
		// 데미지를 받는쪽의 TakeDamage를 호출한다
		OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
	}
	else
		if (OtherActor->ActorHasTag("Player"))
		{
			FDamageEvent damageEvent;
			// 데미지를 받는쪽의 TakeDamage를 호출한다
			OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
		}
	handAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AEnemyZombiePawn::BlinkRed()
{
	if (skMeshComponent)
	{
		if (!originMaterial)
		{
			originMaterial = skMeshComponent->GetMaterial(0);
		}
		if (hitMaterial)
		{
			skMeshComponent->SetMaterial(0, hitMaterial);
		}
		GetWorldTimerManager().SetTimer(damageBlinkTimerHandle, this, &AEnemyZombiePawn::EndBlink, 0.1f, false);
	}
}
void AEnemyZombiePawn::EndBlink()
{
	if (skMeshComponent && originMaterial)
		skMeshComponent->SetMaterial(0, originMaterial);
}
//데미지 받기
float AEnemyZombiePawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currentLocation = GetActorLocation();
	statComponent->SetDamage(damage);
	Hit(DamageCauser, DamageAmount);
	return damage;
}


void AEnemyZombiePawn::Hit(AActor* damageCauser,float DamageAmount)
{
	if (bDie)
		return;
	if (bc->GetValueAsBool(FName("IsAttacking")))
	{
		OnAttackEnd();
	}

	if (hitCurrentTime > 0)
		return;

	//색 변하게
	BlinkRed();
	//부딧친 방향 구하기
	FVector start = GetActorLocation();
	start.Z = 0;
	FVector end = damageCauser->GetActorLocation();
	end.Z = 0;
	direction = (start - end).GetSafeNormal();
	direction.Z = 0.f;
	//맞은 위치에서 카메라를 바라보는 Rotator
	APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FRotator damageRotation2 = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCamera->GetCameraLocation());
	//시작위치 homePos
	uiDamageComponent->SetVisibleDamageUI(damageRotation2, direction, GetActorTransform(), DamageAmount);

	if (statComponent->GetHp()<=0)
	{
		bDie = true;
		Die(damageCauser);
	}

	bHit = true;

	if (animInstance == nullptr)
	{
		animInstance = Cast<UAnimInstanceZombie>(skMeshComponent->GetAnimInstance());
	}
	animInstance->PlayHitMontage();
}

void AEnemyZombiePawn::Die(AActor* damageCauser)
{
	FVector locationTarget = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 1000.0f);
	dieDirection = locationTarget;
	GetController()->UninitializeComponents();
	capsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	skMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	skMeshComponent->SetSimulatePhysics(true);
	skMeshComponent->SetEnableGravity(true);
	
	//부딧친 방향 구하기
	FVector start = currentLocation;
	start.Z = 0;
	FVector end = damageCauser->GetActorLocation() + damageCauser->GetActorForwardVector() * -1.f * 1000.f;
	end.Z = 0;
	FVector newDirection = (start - end).GetSafeNormal() * 5.0f;
	newDirection.Z = 4.0f;
	skMeshComponent->AddImpulse(newDirection * 10000.f);
	//플레이어와 출동 하지 않게
	skMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	//Enemy와 출동 하지 않게
	skMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundDie, GetActorLocation(), 0.5f, 1.0f);
	//UGameplayStatics::PlaySound2D(GetWorld(), SoundDie, 1.0f);
}
