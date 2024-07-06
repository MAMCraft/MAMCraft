// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"
#include "IncreaseHPItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);

    ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
    ChestMesh->SetupAttachment(RootComponent);

    BoxComp->SetBoxExtent(FVector(50.0f));
    BoxComp->SetGenerateOverlapEvents(true);
    BoxComp->SetCollisionProfileName(TEXT("ATreasureChest"));

    //BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATreasureChest::OnChestClicked);
    // 보물상자를 클릭했을 때 OnChestClicked 함수를 호출하도록 설정
    // BoxComp->OnClicked.AddDynamic(this, &ATreasureChest::OnChestClicked);
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATreasureChest::OnChestClicked()
{
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
	//AActor* SpawnedActor = GetWorld()->SpawnActor<AIncreaseHPItem>(ItemToSpawn, SpawnLocation, GetActorRotation(), SpawnParams);

    if (ItemToSpawn)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
        AActor* SpawnedActor = GetWorld()->SpawnActor<AIncreaseHPItem>(ItemToSpawn, SpawnLocation, GetActorRotation(), SpawnParams);
    }
}


