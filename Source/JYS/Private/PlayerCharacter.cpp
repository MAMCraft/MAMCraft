// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "HPWidget.h"
#include "Engine/DamageEvents.h"
#include "Components/ArrowComponent.h"
#include "ArrowAttack.h"
#include "ClickMovePlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>




// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->SetUsingAbsoluteRotation(true);
	springArmComponent->TargetArmLength = 800.0f;
	springArmComponent->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	springArmComponent->bDoCollisionTest = false;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(springArmComponent, USpringArmComponent::SocketName);
	cameraComponent->bUsePawnControlRotation = false;

	isAttacking = false;

	//weapon
	rightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("rightWeaponBox"));
	rightWeaponCollision->SetupAttachment(GetMesh(), FName("R_Arm_armor_end"));
	rightWeaponCollision->SetRelativeScale3D(FVector(0.003f, 0.003f, 0.015f));
	rightWeaponCollision->SetRelativeLocation(FVector(0.0f, -0.4f, 0.0f));
	rightWeaponCollision->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	arrowPositionComp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowPositionComp"));
	arrowPositionComp->SetupAttachment(RootComponent);
	// arrowPositionComp->SetRelativeLocationAndRotation(FVector(), FVector());

	bComboAttacking = false;
	bComboAttackStart = false;
	bComboAttackNext = false;
	comboAttackNumber = 0;

	//LSJ 콤보 공격 적용
	MaxCombo = 2;
	AttackEndComboState();

	//LSJ 인벤토리
	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("INVENTORYCOMPONENT"));

	ConstructorHelpers::FObjectFinder<UMaterialInterface> RedMaterialFinder(TEXT("/Script/Engine.Material'/Game/GameResource/Player/Mesh/MI_DamageRed.MI_DamageRed'"));
	if (RedMaterialFinder.Succeeded())
	{
	UE_LOG(LogTemp, Warning, TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
		RedMaterial = RedMaterialFinder.Object;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	// 캐릭터의 시작 위치와 회전을 저장
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();

	// 초기 부활 횟수 설정
	RespawnCount = 1;

	HPWidget = CreateWidget<UHPWidget>(GetWorld(), HPWidgetFactory);
	if (HPWidget)
	{
		HPWidget->AddToViewport();
	}
	//LSJ 인벤토리
	inventoryHUD = CreateWidget<class UInventoryWidget>(GetWorld(), inventoryHUDFactory);
	if (inventoryHUD)
	{
		inventoryHUD->AddToViewport();
	}
	// Combo Anim 
	AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance()); //LSJ 콤보 공격 적용
	if (AnimInstance != nullptr)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::HandleOnMontageNotifyBegin);
		//LSJ 콤보 공격 적용
		AnimInstance->EndAttack.AddLambda([this]() -> void
			{
				OnAttackComboMontageEnded();
			});
		AnimInstance->OnNextAttackCheck.AddLambda([this]() -> void
			{

				CanNextCombo = false;

				if (IsComboInputOn)
				{
					AttackStartComboState();
					UE_LOG(LogTemp, Error, TEXT("%d"), CurrentCombo);
					//switch (CurrentCombo)
					//{
					//case 1:
					//	//AnimInstance->JumpToAttackMontageSection1();
					//	break;
					//case 2:
					//	//AnimInstance->JumpToAttackMontageSection2();
					//}
					if (CurrentCombo>1)
					{
						//GetMesh()->SocketRo
						//소켓의 ratation
					}
					AnimInstance->JumpToAttackMontageSection(CurrentCombo);
				}
			});
		characterSpeed = GetCharacterMovement()->MaxWalkSpeed ;
	}

	//weapon
	// Setup Right weapon collision box
	rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	rightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// UE_LOG(LogTemp, Log, TEXT("CollisionBox"));
	rightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnrightWeaponCollision);


	// ArrowAttackClass = Cast<AActor>(GetMesh());


}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//weapon
	if (rightWeaponCollision)
	{
		DrawDebugBox(GetWorld(), rightWeaponCollision->GetSocketLocation(FName("R_Arm_armor_end")), rightWeaponCollision->GetScaledBoxExtent(),rightWeaponCollision->GetSocketRotation(FName("R_Arm_armor_end")).Quaternion(), FColor::Red, false, -1.0f, 0, 5.0f);
	}


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("RollingAction", IE_Pressed, this, &APlayerCharacter::Rolling);
	PlayerInputComponent->BindAction("comboAttack", IE_Pressed, this, &APlayerCharacter::comboAttack);
	//PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &APlayerCharacter::comboAttackStart);
	//PlayerInputComponent->BindAction("Skill", IE_Released, this, &APlayerCharacter::comboAttackEnd);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &APlayerCharacter::skill);

}

void APlayerCharacter::Rolling()
{
	UE_LOG(LogTemp, Log, TEXT("rollingMontage"));

	if (rollingMontage)
	{

			/*FVector Destination = GetActorLocation() + GetActorForwardVector() * 120.0f;
			float const Distance = FVector::Dist(Destination,GetActorLocation());
			if (Distance > 120.0f)
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(UGameplayStatics::GetPlayerController(GetWorld(), 0), Destination);
			}*/
		char_controller->SetNewMove(GetActorLocation() + GetActorForwardVector() * 200 );
		PlayAnimMontage(rollingMontage);
		UE_LOG(LogTemp, Log, TEXT("rollingMontage"));
	}
}


void APlayerCharacter::IncreaseHP(int32 Amount)
{
	playerHP = FMath::Clamp(playerHP + Amount, 0, playerMaxHP);

	if (HPWidget)
	{
		HPWidget->SetHP(playerHP, playerMaxHP);
	}

	UE_LOG(LogTemp, Log, TEXT("HP Increased: %d / %d"), playerHP, playerMaxHP);
}

void APlayerCharacter::IncreaseAttackDamage(float Amount)
{
	AttackDamage += Amount;
	UE_LOG(LogTemp, Log, TEXT("Attack Damage Increased: %d"), AttackDamage);
}

void APlayerCharacter::OnMyTakeDamage(int damage)
{
	UE_LOG(	LogTemp, Warning, TEXT("부활~~~~~~~~~~~~~~~~~~~~~~%d"), playerHP)
	// 데미지 만큼 체력을 소모한다.
	playerHP -= damage;
	if (playerHP < damage)
	{
		playerHP = 0;
	}

	// 체력이 결정되었다면 UI로 반영한다.
	if (nullptr != HPWidget)
		HPWidget->SetHP(playerHP, playerMaxHP);

	// 빨간색으로 깜빡이기
	BlinkRed();

	if (playerHP == 0 && RespawnCount > 0)
	{
		Respawn();
		RespawnCount--;
		playerHP = playerMaxHP;
	} else if (playerHP == 0 && RespawnCount <=0) {
			// 부활 횟수 초과시 캐릭터 파괴 
			Destroy();
	}
}

void APlayerCharacter::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingpayload)
{
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageCauser->ActorHasTag(FName("Fire")))
	{
		//5초간 데미지 
		//업데이트
		if (!bIsTakingFireDamage)
		{
			bIsTakingFireDamage = true;
			GetWorldTimerManager().SetTimer(FireDamageTimerHandle, this, &APlayerCharacter::ApplyFireDamage, 1.0f, true, 0.0f);
			GetWorldTimerManager().SetTimer(FireDamageTimerHandle, this, &APlayerCharacter::StopFireDamage, 5.0f, false);
		}
	}
	else
		OnMyTakeDamage(DamageAmount);

	//hit 캐릭터가 빨간색 반짝반짝

	UE_LOG(LogTemp, Warning, TEXT("TakeDamage : %s"), *DamageCauser->GetName())
		return Damage;
}


void APlayerCharacter::skill()
{
	UE_LOG(LogTemp, Log, TEXT("bowMonatge"));
	if (OnSkill)
	{
		return;
	}
	
	PlayAnimMontage(bowMontage);

	// Rotate to mouse direction
	RotateToMouseDirection();

}



void APlayerCharacter::ApplyFireDamage()
{
	OnMyTakeDamage(1);
}

void APlayerCharacter::StopFireDamage()
{
	bIsTakingFireDamage = false;
	GetWorldTimerManager().ClearTimer(FireDamageTimerHandle);
}



void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Hit(OtherActor);
}

void APlayerCharacter::BlinkRed()
{
	// 메쉬 컴포넌트가 유효한지 확인
	if (GetMesh())
	{
		// 원래 머티리얼을 저장 (한 번만 저장)
		if (!OriginalMaterial)
		{
			OriginalMaterial = GetMesh()->GetMaterial(0);
		}

		// 빨간색 머티리얼을 적용
		if (RedMaterial)
		{
			GetMesh()->SetMaterial(0, RedMaterial);
		}

		// 일정 시간 후 원래 머티리얼로 복원
		GetWorldTimerManager().SetTimer(DamageBlinkTimerHandle, this, &APlayerCharacter::EndBlink, 0.5f, false);
	}
}

void APlayerCharacter::EndBlink()
{
	// 원래 머티리얼을 복원
	if (GetMesh() && OriginalMaterial)
	{
		GetMesh()->SetMaterial(0, OriginalMaterial);
	}
}

void APlayerCharacter::OnMyActionArrow()
{
	// Arrow 스폰 위치를 10.f 만큼만 앞으로
	//FVector ArrowSpawnLocation = arrowPositionComp->GetComponentLocation();
	//ArrowSpawnLocation += arrowPositionComp->GetForwardVector() * 30.f;
	FTransform t = arrowPositionComp->GetComponentTransform();
	GetWorld()->SpawnActor<AArrowAttack>(arrowFactory, t);
}

void APlayerCharacter::DelayedArrowAttack()
{
	OnMyActionArrow();
}

void APlayerCharacter::RotateToMouseDirection()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FVector WorldLocation, WorldDirection;
		FVector2D ScreenLocation;
		if (PlayerController->GetMousePosition(ScreenLocation.X, ScreenLocation.Y))
		{
			bool bSuccess = PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection);
			if (bSuccess)
			{
				// 플레이어 캐릭터가 바라봐야 할 위치를 찾음
				FVector LookAtLocation = WorldLocation + (WorldDirection * 1000);  // 방향 벡터를 확장

				// 해당 위치를 바라보는 회전을 계산
				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtLocation);
				SetActorRotation(NewRotation);

				// 화살 공격을 지연시킴
				GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &APlayerCharacter::DelayedArrowAttack, 0.5f, false);
			}
		}
	}
}

void APlayerCharacter::comboAttackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("55555555555"))

	bComboAttackStart = true;

	if (bComboAttacking == false)
	{
		comboAttack();
	}
	else if (bComboAttacking == true)
	{
		bComboAttackNext = true;
	}
}

void APlayerCharacter::comboAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("4444444444444"))

	bComboAttackStart = false;
}
//LSJ 콤보 공격 적용
void APlayerCharacter::comboAttack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		AnimInstance->PlayAttackMontage();
		AnimInstance->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}

}

void APlayerCharacter::comboAttackEndNotice()
{
	UE_LOG(LogTemp, Warning, TEXT("222222222222222"))
	bComboAttacking = false;
	comboAttackNumber = 0;
}

void APlayerCharacter::comboAttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("11111111111111"))
	if (bComboAttackNext == true)
	{
		comboAttackNumber += 1;
		bComboAttackNext = false;
		comboAttack();
	}
}

void APlayerCharacter::Respawn()
{
	// 부활 횟수를 증가시킴
	// RespawnCount++;

	// 캐릭터 위치와 회전을 시작 위치로 설정
	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);

	// HP를 초기화
	playerHP = playerMaxHP;
	if (HPWidget)
	{
		HPWidget->SetHP(playerHP, playerMaxHP);
	}
}

void APlayerCharacter::SetController(AClickMovePlayerController* cont)
{
	char_controller = cont;
}


//weapon
void APlayerCharacter::OnrightWeaponCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnrightWeaponCollision : %s"), *OtherActor->GetName());
	if (OtherActor->ActorHasTag("Enemy"))
	{
		FDamageEvent damageEvent;
		OtherActor->TakeDamage(AttackDamage, damageEvent, GetController(), this);
		}

}

// LSJ 콤보 공격 적용
void APlayerCharacter::OnAttackComboMontageEnded()
{
	IsAttacking = false;
	AttackEndComboState();
}

void APlayerCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void APlayerCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}
