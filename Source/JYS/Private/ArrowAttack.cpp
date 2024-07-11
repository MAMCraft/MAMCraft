// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowAttack.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"

void AArrowAttack::OnrightWeaponCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnrightWeaponCollision : %s"), *OtherActor->GetName());
	if (OtherActor->ActorHasTag("Enemy"))
	{
		FDamageEvent damageEvent;
		OtherActor->TakeDamage(1, damageEvent,nullptr, this);
		Destroy();
	}

}
// Sets default values
AArrowAttack::AArrowAttack()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 루트 컴포넌트로 UBoxComponent를 설정하고, 속성을 초기화합니다.
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);
    BoxComp->SetBoxExtent(FVector(37.5f, 12.5f, 50.f));

    // 화살 메시를 위한 UStaticMeshComponent를 생성하고, 루트 컴포넌트에 부착합니다.
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    // 화살 메시를 생성하고 설정합니다.
    ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));  // 경로 수정

    if (TempMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(TempMesh.Object);
        MeshComp->SetRelativeScale3D(FVector(0.75f, 0.25f, 1));
    }

    // 화살 메시를 오른손 소켓에 부착하도록 설정합니다.
    static FName SocketName(TEXT("R_Hand_Bow"));  // 소켓 이름 설정

    // 소켓이 존재하는지 확인하고 부착합니다.
    if (GetAttachParentActor())
    {
        MeshComp->AttachToComponent(GetAttachParentActor()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
    }
}

// Called when the game starts or when spawned
void AArrowAttack::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArrowAttack::DestroyArrow, 1.0f, true);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AArrowAttack::OnrightWeaponCollision);
}

// Called every frame
void AArrowAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector P0 = GetActorLocation();
	FVector velocity = GetActorForwardVector() * Speed;
	float t = DeltaTime;
	FVector NewLocation = P0 + velocity * t;
	SetActorLocation(NewLocation);
}

void AArrowAttack::DestroyArrow()
{
	Destroy();
}



