// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBlazePawn.h"
#include "AIControllerBlaze.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ActorBlazeBullet.h"
#include "Components/WidgetComponent.h"

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
	//hit material
	ConstructorHelpers::FObjectFinder<UMaterialInterface> hitMaterialFinder(TEXT("/Script/Engine.Material'/Game/LSJ/Resource/Blaze/Mesh/MI_BlazeHit.MI_BlazeHit'"));
	if (hitMaterialFinder.Succeeded())
	{
		hitMaterial = hitMaterialFinder.Object;
	}
	//hpUI
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
	hpBarWidget = Cast<UHpBarWidget>(hpBarWidgetComponent->GetUserWidgetObject());
	if (nullptr != hpBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("BindCharacterStat"));
		hpBarWidget->BindCharacterStat(statComponent);
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
	bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
	groundZValue = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation().Z;

	hpBarWidget = Cast<UHpBarWidget>(hpBarWidgetComponent->GetUserWidgetObject());
	if (nullptr != hpBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("BindCharacterStat"));
		hpBarWidget->BindCharacterStat(statComponent);
	}
}

// Called every frame
void AEnemyBlazePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDie)
	{
		dieCurrentTime += DeltaTime;
		if (dieCurrentTime > dieDestroyTime)
		{
			Destroy();
		}
		if (GetActorLocation().Z - groundZValue < -100.f)
			return;
		//위치 이동 포물선
		//ground까지
		timeDie += DeltaTime;
		Velo_x = velocityDie * hitDirection.X * FMath::Cos(FMath::DegreesToRadians(Rad)) * timeDie;
		Velo_y = velocityDie*100.0 * hitDirection.Z * FMath::Sin(FMath::DegreesToRadians(Rad)) * timeDie;
		double Velo_z = velocityDie * hitDirection.Y * FMath::Cos(FMath::DegreesToRadians(Rad)) * timeDie;
		//핵심 부분입니다.
		double xx = Velo_x;
		double yy = Velo_y - (0.5 * gravityDie * FMath::Pow(timeDie, 2));
		double zz = Velo_z;
		//y값의 경우 중력의 힘을 받기 때문에 필요한 부분입니다.
		//모두 위 공식에 근거하여 식을 대입해보시면 알 수 있습니다.
		fx = (xx);
		fy = (yy);
		float fz = (zz);
		//double을 float으로 바꾸어줍니다.
		// 안 바꾸어주어도 상관은 없지만 저의 경우 float을 써야만 하는 상황이 오기 때문에 굳이 넣어줬었습니다.
		float CurrentX = GetActorLocation().X;
		float CurrentY = GetActorLocation().Z;
		float CurrentZ = GetActorLocation().Y;
		if (CurrentY > MaxH)
		{
			MaxH = CurrentY;
		}
		FVector v = FVector(fx, fz, fy);
		
		SetActorLocation(FVector(fx + GetActorLocation().X, fz + GetActorLocation().Y, fy + GetActorLocation().Z));
		//float angle = FMath::Atan2(fy + PositionY, fx + PositionX) * FMath::RadiansToDegrees(angle);
		//angle 부분은 미사일이 현 상황에 따라 앞 꼬다리 부분의 방향을 말하는 것인데 어느정도 수정이 필요해보이네요 전
		//transform.eulerAngles = new Vector3(0, 0, angle);
	}
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
	bc = UAIBlueprintHelperLibrary::GetBlackboard(this);
	bc->SetValueAsBool(FName("IsAttacking"), false);
	bc->SetValueAsBool(FName("WasRunAway"), false);
	movement->MaxSpeed = 300.f;
}

void AEnemyBlazePawn::Attack(TArray<FVector>& location)
{
	attackLocation = location;
	if (animInstance == nullptr)
	{
		bc->SetValueAsBool(FName("IsAttacking"), false);
		animInstance = Cast<UAnimInstanceBlaze>(skMeshComponent->GetAnimInstance());
		return;
	}
	movement->MaxSpeed = 0;
	animInstance->PlayAttackMontage();
}


float AEnemyBlazePawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	statComponent->SetDamage(damage);
	Hit(DamageCauser);
	return damage;
}

void AEnemyBlazePawn::Hit(AActor* damageCauser)
{
	if (bDie)
		return;
	if (bc->GetValueAsBool(FName("IsAttacking")))
	{
		OnAttackEnd();
	}

	/*if (hitCurrentTime > 0)
		return;*/

	//색 변하게
	BlinkRed();
	//부딧친 방향 구하기
	FVector start = GetActorLocation();
	start.Z = 0;
	FVector end = damageCauser->GetActorLocation();
	end.Z = 0;
	hitDirection = (start - end).GetSafeNormal();
	hitDirection.Z = 0.f;

	if (statComponent->GetHp() <= 0)
	{
		bDie = true;
		Die(damageCauser);
	}

	bHit = true;

	/*if (animInstance == nullptr)
	{
		animInstance = Cast<UAnimInstanceBlaze>(skMeshComponent->GetAnimInstance());
	}
	animInstance->PlayHitMontage();*/
	
}
void AEnemyBlazePawn::Die(AActor* damageCauser)
{
	if (animInstance == nullptr)
	{
		animInstance = Cast<UAnimInstanceBlaze>(skMeshComponent->GetAnimInstance());
	}
	animInstance->PlayDieMontage();
	GetController()->UninitializeComponents();
}

void AEnemyBlazePawn::BlinkRed()
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
		GetWorldTimerManager().SetTimer(damageBlinkTimerHandle, this, &AEnemyBlazePawn::EndBlink, 0.1f, false);
	}
}
void AEnemyBlazePawn::EndBlink()
{
	if (skMeshComponent && originMaterial)
		skMeshComponent->SetMaterial(0, originMaterial);
}