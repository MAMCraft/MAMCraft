// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"
#include "IncreaseHPItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include <GameFramework/Character.h>

// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);

    // ���̷�Ż �޽� ������Ʈ ����
    TreasureMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TreasureMesh"));
    TreasureMesh->SetupAttachment(RootComponent);
    TreasureMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

    // ���̷�Ż �޽� �ε�
    //static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Path/To/Your/SkeletalMesh.SkeletalMesh"));
    //if (MeshAsset.Succeeded())
    //{
    //    TreasureMesh->SetSkeletalMesh(MeshAsset.Object);
    //}

    // �ִϸ��̼� ��Ÿ�� �ε�
    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/GameResource/Player/GoldChest/AM_GoldChest_Anim_Montage.AM_GoldChest_Anim_Montage'"));
    if (MontageAsset.Succeeded())
    {
        OpenChestMontage = MontageAsset.Object;
    }

    BoxComp->SetBoxExtent(FVector(90.0f, 50.0f, 50.0f));
    BoxComp->SetGenerateOverlapEvents(true);
    BoxComp->SetCollisionProfileName(TEXT("TreasureChest"));

    BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FClassFinder<AIncreaseHPItem> increaseHPItem(TEXT("/Script/Engine.Blueprint'/Game/JYS/Blueprints/BP_IncreaseHPItem.BP_IncreaseHPItem_C'"));
    if (increaseHPItem.Succeeded())
    {  
        ItemToSpawn = increaseHPItem.Class;
    }

    OnlyOnce = false;
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	

    // �ʱ� ���¸� Idle�� ����
    if (TreasureMesh)
    {
        UAnimInstance* AnimInstance = TreasureMesh->GetAnimInstance();
        if (AnimInstance && OpenChestMontage)
        {
            // �ִϸ��̼� �ʱ�ȭ�� ���� �ִϸ��̼� ��Ÿ�ָ� �Ͻ������� ����
            AnimInstance->Montage_Stop(0.0f, OpenChestMontage);
        }
    }

}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    
}

void ATreasureChest::OnChestClicked()
{
    if (OnlyOnce) {
        return;
    }
    OnlyOnce = true;
    // �ִϸ��̼� ��Ÿ�� ���
    if (OpenChestMontage && TreasureMesh)
    {
        UAnimInstance* AnimInstance = TreasureMesh->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(OpenChestMontage);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Item Spawn!!!!!!!!!!!!!!!!"));
    UE_LOG(LogTemp, Warning, TEXT("Item!!!!!!!!!!!!!"));

    GetWorldTimerManager().SetTimer(SpawnItemTimerHandle, this, &ATreasureChest::SpawnItemAfterDelay, 1.0f, false);

}

void ATreasureChest::SpawnItemAfterDelay()
{
    // ������ ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 200.0f, 0.0f);
    AActor* SpawnedActor = GetWorld()->SpawnActor<AIncreaseHPItem>(ItemToSpawn, SpawnLocation, GetActorRotation(), SpawnParams);

    UE_LOG(LogTemp, Warning, TEXT("Item Spawned after delay!"));
}


