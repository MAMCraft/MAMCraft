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



// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled =true;

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

	//// �÷��̾� �տ� �ڽ� ���� (Create and attach the overlap box)
	//overlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	//overlapBox->SetupAttachment(RootComponent);

	//// �ڽ� ���� (Set the size and position of the overlap box)
	//overlapBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	//overlapBox->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	//// overlap bind�ϱ� (Bind the overlap event)
	//overlapBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	// ������ ��ŭ ü���� �Ҹ��Ѵ�.
	playerHP -= damage;
	if (playerHP < damage)
	{
		playerHP = 0;
	}

	// ü���� �����Ǿ��ٸ� UI�� �ݿ��Ѵ�.
	if (nullptr != HPWidget)
		HPWidget->SetHP(playerHP, playerMaxHP);

	// ���� ü���� 0�̸� �״´�.
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

// �÷��̾ ���񿡰� ������ �ޱ�
void APlayerCharacter::Hit(AActor* OtherActor)
{

}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Hit(OtherActor);
}





