// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.

#include "ArrowAttack.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AArrowAttack::AArrowAttack()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 루트 컴포넌트로 UBoxComponent를 설정
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);
    BoxComp->SetBoxExtent(FVector(58.0f, 5.0f, 15.f));

    // 화살 메시 설정
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    // 화살 메시 로드
    ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/GameResource/Player/Arrow/arrowBullet.arrowBullet'"));
    if (TempMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(TempMesh.Object);
        MeshComp->SetRelativeScale3D(FVector(0.75f, 0.25f, 1));
        MeshComp->SetRelativeLocation(FVector(30.0f, 0.0f, 0.0f));
    }

    // 소켓 이름 설정
    static FName SocketName(TEXT("R_Hand_Bow"));

    // 부모 액터 확인 및 소켓에 부착
    if (GetAttachParentActor())
    {
        USkeletalMeshComponent* ParentMesh = Cast<USkeletalMeshComponent>(GetAttachParentActor()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
        if (ParentMesh && ParentMesh->DoesSocketExist("R_Hand_Bow"))
        {
            MeshComp->AttachToComponent(ParentMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
        }
    }

    // 파티클 시스템 로드
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/ Script / Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
    if (ParticleSystem.Succeeded())
    {
        HitEffect = ParticleSystem.Object;
    }
}

// Called when the game starts or when spawned
void AArrowAttack::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArrowAttack::DestroyArrow, 1.0f, true);
    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AArrowAttack::OnrightWeaponCollision);

    // 소켓 이름 설정
    static FName SocketName(TEXT("R_Hand_Bow"));
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

void AArrowAttack::OnrightWeaponCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("OnrightWeaponCollision : %s"), *OtherActor->GetName());
    if (OtherActor->ActorHasTag("Enemy"))
    {
        FDamageEvent damageEvent;
        OtherActor->TakeDamage(1, damageEvent, nullptr, this);

        // 이펙트를 생성하는 부분
        if (HitEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, OtherActor->GetActorLocation(), FRotator::ZeroRotator);
        }

        Destroy();
    }
}

void AArrowAttack::DestroyArrow()
{
    Destroy();
}



