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

	// 플레이어 앞에 박스 생성 (Create and attach the overlap box)
	overlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	overlapBox->SetupAttachment(RootComponent);

	// 박스 설정 (Set the size and position of the overlap box)
	overlapBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	overlapBox->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	// overlap bind하기 (Bind the overlap event)
	overlapBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	

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
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::attack);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &APlayerCharacter::skill);
}

void APlayerCharacter::Rolling()
{
	UE_LOG(LogTemp, Log, TEXT("rollingMonatge"));
	
	if (rollingMonatge)
	{
		PlayAnimMontage(rollingMonatge);
		UE_LOG(LogTemp, Log, TEXT("rollingMonatge"));
	}
}

void APlayerCharacter::attack()
{
	UE_LOG(LogTemp, Log, TEXT("Random Attack"));

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (attackMontages.Num() > 0)
	{
		int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, attackMontages.Num() - 1);

		if (AnimInstance && attackMontages[RandomIndex])
		{
			AnimInstance->Montage_Play(attackMontages[RandomIndex]);
		}
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

// 오버랩 되면 destroy되게끔
void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// 오버랩 된 액터들을 Destroy한다
		OtherActor->Destroy();
		UE_LOG(LogTemp, Log, TEXT("Destroy"), *OtherActor->GetName());
	}
}


