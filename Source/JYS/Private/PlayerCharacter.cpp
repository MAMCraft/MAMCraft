// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "HPWidget.h"
#include "Engine/DamageEvents.h"



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

	//// 플레이어 앞에 박스 생성 (Create and attach the overlap box)
	//overlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	//overlapBox->SetupAttachment(RootComponent);

	//// 박스 설정 (Set the size and position of the overlap box)
	//overlapBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	//overlapBox->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	//// overlap bind하기 (Bind the overlap event)
	//overlapBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

	//weapon
	rightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("rightWeaponBox"));
	rightWeaponCollision->SetupAttachment(GetMesh(), FName("rightWeaponBone"));
	rightWeaponCollision->SetRelativeScale3D(FVector(0.01f));
	rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	// Combo Anim
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::HandleOnMontageNotifyBegin);
	}

	//weapon
	// Setup Right weapon collision box
	rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	rightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	rightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// UE_LOG(LogTemp, Log, TEXT("CollisionBox"));
	rightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnrightWeaponCollision);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//weapon
	if (rightWeaponCollision)
	{
		DrawDebugBox(GetWorld(), rightWeaponCollision->GetComponentLocation(), rightWeaponCollision->GetScaledBoxExtent(), FColor::Red, false, -1.0f, 0, 5.0f);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("RollingAction", IE_Pressed, this, &APlayerCharacter::Rolling);
	PlayerInputComponent->BindAction("comboAttack", IE_Pressed, this, &APlayerCharacter::comboAttack);
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
	ComboAttackIndex--;

	if (ComboAttackIndex < 0)
	{
		// Get Anim Instance
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Stop(0.4f, attackComboMontage);
			isAttacking = false;
		}
	}

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

void APlayerCharacter::comboAttack()
{
	//stop Montage if below zero
	if (!isAttacking)
	{
		// Get the animation instance
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			// Play Light Attack
			if (attackComboMontage != nullptr)
			{
				// AnimInstance->Montage_Play
				PlayAnimMontage(attackComboMontage);
				isAttacking = true; // Set attacking flag
				UE_LOG(LogTemp, Log, TEXT("Playing attack combo montage"));
			}
			else
			{
				ComboAttackIndex++;
				AnimInstance->Montage_JumpToSection(FName("NextCombo"), attackComboMontage);
				UE_LOG(LogTemp, Log, TEXT("Jumping to next combo section"));
			}
		}
	}
	else {
		ComboAttackIndex = 1;
		UE_LOG(LogTemp, Log, TEXT("ComboAttackIndex set to 1"));
	}

}


void APlayerCharacter::skill()
{
	UE_LOG(LogTemp, Log, TEXT("bowMonatge"));

	if (bowMontage)
	{
		PlayAnimMontage(bowMontage);
		UE_LOG(LogTemp, Log, TEXT("bowMonatge"));
	}
}

// 플레이어가 좀비에게 데미지 받기
void APlayerCharacter::Hit(AActor* OtherActor)
{

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

void APlayerCharacter::FlashRed()
{
}

void APlayerCharacter::ResetMaterial()
{
}


void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Hit(OtherActor);
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