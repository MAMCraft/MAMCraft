// Fill out your copyright notice in the Description page of Project Settings.


#include "IncreaseHPItem.h"
#include "Components/StaticMeshComponent.h"
#include <PlayerCharacter.h>

// Sets default values
AIncreaseHPItem::AIncreaseHPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	this->SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	boxComp->SetBoxExtent(FVector(20.0f));
	boxComp->SetRelativeScale3D(FVector(1.0f));

	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionProfileName(TEXT("Item"));

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AIncreaseHPItem::OnMyCompBeginOverlap);


}

void AIncreaseHPItem::OnMyCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("xxxxxx"));

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// HP를 10 증가시킴
		PlayerCharacter->IncreaseHP(3);

		// 아이템을 파괴하여 소모되게 함
		Destroy();
	}
}

// Called when the game starts or when spawned
void AIncreaseHPItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AIncreaseHPItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

