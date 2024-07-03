// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include <LSJ/Public/EnemyZombiePawn.h>

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Right weapon collision box
	rightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("rightWeaponBox"));
	rightWeaponCollision->SetupAttachment(GetMesh(), FName("rightWeaponBone"));
	rightWeaponCollision->SetRelativeScale3D(FVector(0.01f));

}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup Right weapon collision box
	rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	rightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	rightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// UE_LOG(LogTemp, Log, TEXT("CollisionBox"));
	rightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnrightWeaponCollision);
}



// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (rightWeaponCollision)
	{
		DrawDebugBox(GetWorld(), rightWeaponCollision->GetComponentLocation(), rightWeaponCollision->GetScaledBoxExtent(), FColor::Red, false, -1.0f, 0, 5.0f);
	}
}

// Called to bind functionality to input
void APlayerWeapon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerWeapon::OnrightWeaponCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("zzzzzz"));
}

