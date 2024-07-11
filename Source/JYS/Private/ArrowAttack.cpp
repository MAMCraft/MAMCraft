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

    // ��Ʈ ������Ʈ�� UBoxComponent�� �����ϰ�, �Ӽ��� �ʱ�ȭ�մϴ�.
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);
    BoxComp->SetBoxExtent(FVector(37.5f, 12.5f, 50.f));

    // ȭ�� �޽ø� ���� UStaticMeshComponent�� �����ϰ�, ��Ʈ ������Ʈ�� �����մϴ�.
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    // ȭ�� �޽ø� �����ϰ� �����մϴ�.
    ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));  // ��� ����

    if (TempMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(TempMesh.Object);
        MeshComp->SetRelativeScale3D(FVector(0.75f, 0.25f, 1));
    }

    // ȭ�� �޽ø� ������ ���Ͽ� �����ϵ��� �����մϴ�.
    static FName SocketName(TEXT("R_Hand_Bow"));  // ���� �̸� ����

    // ������ �����ϴ��� Ȯ���ϰ� �����մϴ�.
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



