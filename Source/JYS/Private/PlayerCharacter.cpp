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
	rightWeaponCollision->SetRelativeLocation(FVector(0.0f, 0.0f, -0.4f));
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

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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
						
					}
					AnimInstance->JumpToAttackMontageSection(CurrentCombo);
				}
			});
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
		DrawDebugBox(GetWorld(), rightWeaponCollision->GetComponentLocation(), rightWeaponCollision->GetScaledBoxExtent(), GetMesh()->GetSocketRotation(FName("R_Arm_armor_end")).Quaternion(), FColor::Red, false, -1.0f, 0, 5.0f);
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
	// 데미지 만큼 체력을 소모한다.
	playerHP -= damage;
	if (playerHP < damage)
	{
		playerHP = 0;
	}

	// 체력이 결정되었다면 UI로 반영한다.
	if (nullptr != HPWidget)
		HPWidget->SetHP(playerHP, playerMaxHP);

	// 만약 체력이 0이면 죽는다.
	if (playerHP == 0)
	{
		Destroy();
	}
}

void APlayerCharacter::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingpayload)
{
	// Decrement Combo Index
	//ComboAttackIndex--;

	//if (ComboAttackIndex < 0)
	//{
	//	// Get Anim Instance
	//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//	if (AnimInstance != nullptr)
	//	{
	//		AnimInstance->Montage_Stop(0.4f, attackComboMontage);
	//		isAttacking = false;
	//	}
	//}

}
//공격 콜리전 끄고 키기
//적을 선택했을때 공격애니메이션이 나가게
//공격애니메이션 시작지점에 키고
//끝나는 지점에 끄고
//한번만 beginoverlap

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

		PlayAnimMontage(bowMontage);
		// 이동 애니메이션 재생
		RotateToMouseDirection(); // 마우스 방향으로 회전

		UE_LOG(LogTemp, Log, TEXT("bowMontage"));
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
	//UE_LOG(LogTemp, Warning, TEXT("3333333333333"))

	//bComboAttackStart = true;

	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (!AnimInstance || !attackComboMontage) return;

	//bComboAttacking = true;
	//const char* comboList[] = {"firstAttack", "secondAttack", "thirdAttack"};

	//if (comboAttackNumber >= 2)
	//	comboAttackNumber = 0;

	//AnimInstance->Montage_Play(attackComboMontage, 1.5f);
	//AnimInstance->Montage_JumpToSection(FName(comboList[comboAttackNumber]), attackComboMontage);
	//UE_LOG(LogTemp, Warning, TEXT("9999999999999999"))


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
//