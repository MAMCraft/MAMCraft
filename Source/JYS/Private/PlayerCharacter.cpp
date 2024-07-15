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
#include <RespawnWidget.h>
#include "MAMCraft/Public/FailScreen.h"




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
	springArmComponent->SetupAttachment(GetMesh());
	springArmComponent->SetUsingAbsoluteRotation(true);
	springArmComponent->TargetArmLength = 1400.0f;
	springArmComponent->SetRelativeRotation(FRotator(-56.0f, 45.0f, 0.0f));
	springArmComponent->bDoCollisionTest = false;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(springArmComponent, USpringArmComponent::SocketName);
	cameraComponent->bUsePawnControlRotation = false;

	isAttacking = false;

	//weapon
	swordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));
	rightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("rightWeaponBox"));
	rightWeaponCollision->SetupAttachment(GetMesh(), FName("R_Arm_armor_end"));
	rightWeaponCollision->SetRelativeScale3D(FVector(0.003f, 0.003f, 0.015f));
	rightWeaponCollision->SetRelativeLocation(FVector(0.0f, -0.4f, 0.0f));
	rightWeaponCollision->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	swordMesh->SetupAttachment(GetMesh(), FName("R_Arm_armor_end"));
	swordMesh->SetRelativeScale3D(FVector(0.003f, 0.003f, 0.015f));
	swordMesh->SetRelativeLocation(FVector(0.0f, -0.4f, 0.0f));
	swordMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	swordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	arrowPositionComp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowPositionComp"));
	arrowPositionComp->SetupAttachment(RootComponent);
	// arrowPositionComp->SetRelativeLocationAndRotation(FVector(), FVector());

	bowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bowStaticMesh"));
	bowMesh->SetupAttachment(GetMesh(), FName("R_Arm_armor_end"));
	bowMesh->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	bowMesh->SetRelativeLocation(FVector(0.018f, -0.3f, 0.0f));
	bowMesh->SetRelativeRotation(FRotator(0.0f, 80.0f, 180.0f));
	bowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �޽����� ������ٰ� �־��ٰ�..
	bowMesh->SetVisibility(false);
	swordMesh->SetVisibility(true);

	bComboAttacking = false;
	bComboAttackStart = false;
	bComboAttackNext = false;
	comboAttackNumber = 0;

	MaxCombo = 2;
	AttackEndComboState();

	//LSJ �κ��丮
	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("INVENTORYCOMPONENT"));

	// ������ ������ ���������� ����
	ConstructorHelpers::FObjectFinder<UMaterialInterface> RedMaterialFinder(TEXT("/Script/Engine.Material'/Game/GameResource/Player/Mesh/MI_DamageRed.MI_DamageRed'"));
	if (RedMaterialFinder.Succeeded())
	{
	UE_LOG(LogTemp, Warning, TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
		RedMaterial = RedMaterialFinder.Object;
	}

	bIsTakingFireDamage = false;
	FireDamageTimerHandle.Invalidate();
	StopFireDamageTimerHandle.Invalidate();

	bowState = 0;
	arrowCount = 100;

	static ConstructorHelpers::FClassFinder<UUserWidget> RespawnWidgetBPClass(TEXT("/Game/UI/RespawnWidgetBP"));
	if (RespawnWidgetBPClass.Class != nullptr)
	{
		RespawnWidgetClass = RespawnWidgetBPClass.Class;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FireDamageTimerHandle.Invalidate();
	StopFireDamageTimerHandle.Invalidate();

	// ĳ������ ���� ��ġ�� ȸ���� ����
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();

	// �ʱ� ��Ȱ Ƚ�� ����
	RespawnCount = 1;

	HPWidget = CreateWidget<UHPWidget>(GetWorld(), HPWidgetFactory);
	if (HPWidget)
	{
		HPWidget->AddToViewport();
	}

	if (RespawnWidgetClass)
	{
		RespawnWidget = CreateWidget<URespawnWidget>(GetWorld(), RespawnWidgetClass);
		if (RespawnWidget)
		{
			RespawnWidget->AddToViewport();
			RespawnWidget->SetRespawnCount(RespawnCount, MaxRespawns);
			UE_LOG(LogTemp, Log, TEXT("RespawnWidget added to viewport with RespawnCount: %d, MaxRespawns: %d"), RespawnCount, MaxRespawns);


		}
		
	}
	

	//LSJ �κ��丮
	inventoryHUD = CreateWidget<class UInventoryWidget>(GetWorld(), inventoryHUDFactory);
	if (inventoryHUD)
	{
		inventoryHUD->AddToViewport();
	}
	// Combo Anim 
	AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance()); //LSJ �޺� ���� ����
	if (AnimInstance != nullptr)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::HandleOnMontageNotifyBegin);
		//LSJ �޺� ���� ����
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
						//������ ratation
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

	//if (FailScreenWidgetClass)
	//{
	//	FailScreenWidget = CreateWidget<UFailScreen>(GetWorld(), FailScreenWidgetClass);
	//	if (FailScreenWidget)
	//	{
	//		FailScreenWidget->AddToViewport();
	//		// �⺻ �̹��� ���� (������Ʈ�� �´� �̹����� �����;� ��)
	//		UTexture2D* YouDiedImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/YourPath/YouDiedImage"));
	//		if (YouDiedImage)
	//		{
	//			FailScreenWidget->SetDeathImage(YouDiedImage);
	//		}
	//	}
	//}
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
	// Check if cooldown is active
	if (bIsHPCooldownActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("HP item is on cooldown."));
		return;
	}

	// Increase HP
	playerHP = FMath::Clamp(playerHP + Amount, 0, playerMaxHP);

	// Update HP widget if available
	if (HPWidget)
	{
		HPWidget->SetHP(playerHP, playerMaxHP);
	}

	UE_LOG(LogTemp, Log, TEXT("HP Increased: %d / %d"), playerHP, playerMaxHP);

	// Start cooldown
	
	if (Amount != 10)
	{
		bIsHPCooldownActive = true;
		GetWorld()->GetTimerManager().SetTimer(HPCooldownTimerHandle, this, &APlayerCharacter::ResetHPCooldown, 60.0f, false);
	}
		

	UE_LOG(LogTemp, Log, TEXT("HP item used. Cooldown started."));
}

void APlayerCharacter::IncreaseAttackDamage(float Amount)
{
	AttackDamage += Amount;
	UE_LOG(LogTemp, Log, TEXT("Attack Damage Increased: %d"), AttackDamage);
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

	// ���������� �����̱�
	BlinkRed();

	if (playerHP == 0 && RespawnCount > 0)
	{
		Respawn();
		RespawnCount--;
		playerHP = playerMaxHP;
	} else if (playerHP == 0 && RespawnCount <=0) {
			// ��Ȱ Ƚ�� �ʰ��� ĳ���� �ı� 
			Destroy();
	}
}

void APlayerCharacter::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingpayload)
{
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// �������� �޴� ���
	if (ActualDamage > 0.0f)
	{
		// ������ ����
		OnMyTakeDamage(ActualDamage);

		// Fire �±׸� ���� ���Ϳ��Լ� �������� ���� ���
		if (DamageCauser->ActorHasTag(FName("Fire")))
		{
			if (!bIsTakingFireDamage)
			{
				bIsTakingFireDamage = true;
				UE_LOG(LogTemp, Warning, TEXT("Started taking fire damage"));

				// Ÿ�̸� ����: 0.5�ʸ��� ApplyFireDamage �Լ� ȣ��
				GetWorldTimerManager().SetTimer(FireDamageTimerHandle, this, &APlayerCharacter::ApplyFireDamage, 0.1f, true, 0.0f);

				// 5�� �Ŀ� StopFireDamage �Լ� ȣ��
				GetWorldTimerManager().SetTimer(StopFireDamageTimerHandle, this, &APlayerCharacter::StopFireDamage, 5.0f, false);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("TakeDamage : %s"), *DamageCauser->GetName());
	return ActualDamage;
}


void APlayerCharacter::UpgradeSkill()
{


	if (bowState == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%d UpgradeSkill"), bowState);
		// Fire 1 arrow
		FVector t = rightWeaponCollision->GetComponentLocation();
		AArrowAttack* Arrow = GetWorld()->SpawnActor<AArrowAttack>(t, GetActorForwardVector().Rotation());
		//FireArrow(GetActorForwardVector());
	}
	else if (bowState == 1)
	{
		UE_LOG(LogTemp, Log, TEXT("%d UpgradeSkill"),bowState);
		// ���� ���� ���͸� �������� �������� 30�� ȸ����Ű�� ���� Rotator ����
		FRotator Left30DegreesRotator = FRotator(0.0f, -30.0f, 0.0f);
		FRotator Right30DegreesRotator = FRotator(0.0f, 30.0f, 0.0f);
		// ���� ���� ���͸� FRotator�� ��ȯ
		FRotator CurrentDirectionRotator = GetActorRotation();
		CurrentDirectionRotator.Roll=0;
		CurrentDirectionRotator.Pitch = 0;

		// ���� ���⿡ 30�� ȸ���� ����
		FRotator NewDirectionRotator = UKismetMathLibrary::ComposeRotators(CurrentDirectionRotator, Left30DegreesRotator);
		FRotator NewDirectionRotator2 = UKismetMathLibrary::ComposeRotators(CurrentDirectionRotator, Right30DegreesRotator);
		// ȸ���� Rotator�� FVector�� ��ȯ�ϰ�, Normalize�Ͽ� ��ȯ
		FVector NewDirection = NewDirectionRotator.Vector();
		FVector NewDirection2 = NewDirectionRotator2.Vector();
		//FVector NewDirection = Left30DegreesRotator.Vector();

		FVector t = rightWeaponCollision->GetComponentLocation();
		//FVector forward = FVector(200.0,0.0f,0.f);
		//t = t + forward * GetActorForwardVector();
		AArrowAttack* Arrow1 = GetWorld()->SpawnActor<AArrowAttack>(t, GetActorForwardVector().Rotation());
		AArrowAttack* Arrow3 = GetWorld()->SpawnActor<AArrowAttack>(t, NewDirection2.Rotation());
		AArrowAttack* Arrow2 = GetWorld()->SpawnActor<AArrowAttack>(t, NewDirection.Rotation());
		
			//FireArrow(GetActorForwardVector());
			//FireArrow(NewDirection.GetSafeNormal());
			//FireArrow(GetActorForwardVector() + FVector(0.0f, FMath::RandRange(-0.2f, 0.2f), FMath::RandRange(-0.2f, 0.2f)));
		
	}

	arrowCount -= 1;
}

void APlayerCharacter::FireArrow(const FVector& Direction)
{

	if (arrowPositionComp)
	{
		FTransform ArrowSpawnTransform = arrowPositionComp->GetComponentTransform();
		AArrowAttack* Arrow = GetWorld()->SpawnActor<AArrowAttack>(ArrowSpawnTransform.GetLocation(), Direction.Rotation());
		/*	if (Arrow)
			{
				Arrow->SetActorRotation(Direction.Rotation());
			}*/
	}
}

void APlayerCharacter::skill()
{
	if (inventoryComponent->GetArrow() <= 0)
		return;
	UE_LOG(LogTemp, Log, TEXT("bowMonatge"));
	if (OnSkill)
	{
		return;
	}
	if (arrowCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No arrows left!"));
		return;
	}
	bowState = (int)inventoryComponent->GetCurrentBow();
	PlayAnimMontage(bowMontage);

	// Rotate to mouse direction
	RotateToMouseDirection();
	inventoryComponent->ArrowUsed(1);
}



void APlayerCharacter::ApplyFireDamage()
{
	// Fire ������ ����
	OnMyTakeDamage(1);

	// �α� �����
	UE_LOG(LogTemp, Warning, TEXT("Applying fire damage"));
}


void APlayerCharacter::StopFireDamage()
{
	// Fire ������ ����
	bIsTakingFireDamage = false;

	// Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(FireDamageTimerHandle);
	GetWorldTimerManager().ClearTimer(StopFireDamageTimerHandle);

	// �α� �����
	UE_LOG(LogTemp, Warning, TEXT("Stopped taking fire damage"));
}

void APlayerCharacter::UpdateRespawnUI()
{
	if (RespawnWidget)
	{
		RespawnWidget->SetRespawnCount(RespawnCount, MaxRespawns);
	}
}

void APlayerCharacter::ResetHPCooldown()
{
	bIsHPCooldownActive = false;
	UE_LOG(LogTemp, Log, TEXT("HP item cooldown finished."));
}



void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Hit(OtherActor);
}

void APlayerCharacter::BlinkRed()
{
	// �޽� ������Ʈ�� ��ȿ���� Ȯ��
	if (GetMesh())
	{
		// ���� ��Ƽ������ ���� (�� ���� ����)
		if (!OriginalMaterial)
		{
			OriginalMaterial = GetMesh()->GetMaterial(0);
		}

		// ������ ��Ƽ������ ����
		if (RedMaterial)
		{
			GetMesh()->SetMaterial(0, RedMaterial);
		}

		// ���� �ð� �� ���� ��Ƽ����� ����
		GetWorldTimerManager().SetTimer(DamageBlinkTimerHandle, this, &APlayerCharacter::EndBlink, 0.3f, false);
	}
}

void APlayerCharacter::EndBlink()
{
	// ���� ��Ƽ������ ����
	if (GetMesh() && OriginalMaterial)
	{
		GetMesh()->SetMaterial(0, OriginalMaterial);
	}
}

void APlayerCharacter::OnMyActionArrow()
{
	// Arrow ���� ��ġ�� 10.f ��ŭ�� ������
	//FVector ArrowSpawnLocation = arrowPositionComp->GetComponentLocation();
	//ArrowSpawnLocation += arrowPositionComp->GetForwardVector() * 30.f;
	FTransform t = arrowPositionComp->GetComponentTransform();
	FVector a = t.GetLocation();
	a.Y+= 50.0f;
	t.SetLocation(a);
	GetWorld()->SpawnActor<AArrowAttack>(arrowFactory, t);
}

void APlayerCharacter::DelayedArrowAttack()
{
	UpgradeSkill();
	//OnMyActionArrow();
}

void APlayerCharacter::RotateToMouseDirection()
{
	auto* pc = Cast<AClickMovePlayerController>(Controller);
	if (pc)
	{
		FHitResult hitInfo;
		pc->GetHitResultUnderCursor(ECC_Visibility, false, hitInfo);

		if (hitInfo.GetActor())
		{
			FVector LookAtLocation = hitInfo.ImpactPoint;  // ���� ���͸� Ȯ��

			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtLocation);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(pc, LookAtLocation);

			SetActorRotation(NewRotation);

			// ȭ�� ������ ������Ŵ
			GetWorldTimerManager().SetTimer(SkillTimerHandle, this, &APlayerCharacter::DelayedArrowAttack, 0.5f, false);
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
//LSJ �޺� ���� ����
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

	//if (RespawnCount == 0)
	//{
	//	// ���� ȭ�� ������ 'YouDied' �̹��� ����
	//	// ������Ʈ�� �´� �̹����� �����ͼ� �����ؾ� ��
	//	UTexture2D* YouDiedImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/YourPath/YouDiedImage"));
	//	if (FailScreenWidget && YouDiedImage)
	//	{
	//		FailScreenWidget->SetDeathImage(YouDiedImage);
	//	}
	//}
	//else if (RespawnCount == 1)
	//{
	//	// ���� ȭ�� ������ 'LastChance' �̹��� ����
	//	// ������Ʈ�� �´� �̹����� �����ͼ� �����ؾ� ��
	//	UTexture2D* LastChanceImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/YourPath/LastChanceImage"));
	//	if (FailScreenWidget && LastChanceImage)
	//	{
	//		FailScreenWidget->SetDeathImage(LastChanceImage);
	//	}
	//}
	// ĳ���� ��ġ�� ȸ���� ���� ��ġ�� ����
	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);

	// HP�� �ʱ�ȭ
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

void APlayerCharacter::HideBowMesh()
{
	bowMesh->SetVisibility(false);
}

void APlayerCharacter::ShowSwordMesh()
{
	swordMesh->SetVisibility(true);
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

// LSJ �޺� ���� ����
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
