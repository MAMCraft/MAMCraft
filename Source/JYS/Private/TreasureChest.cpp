#include "TreasureChest.h"
#include "IncreaseHPItem.h"
#include "ArrowItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "ItemBowBubble.h"
// Sets default values
ATreasureChest::ATreasureChest()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);

    // 스켈레탈 메쉬 컴포넌트 생성
    TreasureMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TreasureMesh"));
    TreasureMesh->SetupAttachment(RootComponent);
    TreasureMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

    // (X = 540.000000, Y = 1400.000000, Z = 60.000000)

    // 애니메이션 몽타주 로드
    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAsset(TEXT("/Game/GameResource/Player/GoldChest/AM_GoldChest_Anim_Montage.AM_GoldChest_Anim_Montage"));
    if (MontageAsset.Succeeded())
    {
        OpenChestMontage = MontageAsset.Object;
    }

    BoxComp->SetBoxExtent(FVector(90.0f, 50.0f, 50.0f));
    BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 55.0f));
    BoxComp->SetGenerateOverlapEvents(true);
    BoxComp->SetCollisionProfileName(TEXT("TreasureChest"));

    BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FClassFinder<AIncreaseHPItem> increaseHPItem(TEXT("/Game/JYS/Blueprints/BP_IncreaseHPItem.BP_IncreaseHPItem_C"));
    if (increaseHPItem.Succeeded())
    {
        IncreaseHPItems.Add(increaseHPItem.Class);
    }

    static ConstructorHelpers::FClassFinder<AArrowItem> arrowItem(TEXT("/Game/JYS/Blueprints/BP_ArrowItem.BP_ArrowItem_C"));
    if (arrowItem.Succeeded())
    {
        ArrowItems.Add(arrowItem.Class);
    }
    static ConstructorHelpers::FClassFinder<AItemBowBubble> bowItemFinder(TEXT("/Game/LSJ/Blueprints/Inventory/BP_BowBubble.BP_BowBubble_C"));
    if (bowItemFinder.Succeeded())
    {
        bowItem =(bowItemFinder.Class);
    }
    OnlyOnce = false;
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
    Super::BeginPlay();

    if (TreasureMesh)
    {
        UAnimInstance* AnimInstance = TreasureMesh->GetAnimInstance();
        if (AnimInstance && OpenChestMontage)
        {
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

    if(itemState!=2)
        // ItemState를 클릭 시 무작위로 설정
        itemState = FMath::RandRange(0, 1);
    UE_LOG(LogTemp, Warning, TEXT("Random ItemState: %d"), itemState);

    GetWorldTimerManager().SetTimer(SpawnItemTimerHandle, this, &ATreasureChest::SpawnItemAfterDelay, 1.0f, false);
}

void ATreasureChest::SpawnItemAfterDelay()
{
    TSubclassOf<AActor> ItemToSpawn = nullptr;

    // ItemState에 따라 아이템 리스트에서 랜덤으로 아이템 선택
    if (itemState == 0 && IncreaseHPItems.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, IncreaseHPItems.Num() - 1);
        ItemToSpawn = IncreaseHPItems[RandomIndex];
    }
    else if (itemState == 1 && ArrowItems.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, ArrowItems.Num() - 1);
        ItemToSpawn = ArrowItems[RandomIndex];
    }
    else if(itemState == 2)
    {
        ItemToSpawn = bowItem;
    }

    if (ItemToSpawn)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        // FVector SpawnLocation = GetActorLocation() + FVector(-50.0f, -200.0f, 0.0f);
        FVector SpawnLocation = GetActorLocation() + GetActorRightVector() * 200.0f;
        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemToSpawn, SpawnLocation, GetActorRotation(), SpawnParams);
    }
}
// (X = 766.030739, Y = 1844.202016, Z = 60.000000)