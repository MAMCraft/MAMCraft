// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstEquipmentItem.h"
#include <PlayerCharacter.h>

// Sets default values
AFirstEquipmentItem::AFirstEquipmentItem()
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

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AFirstEquipmentItem::OnMyCompBeginOverlap);
}

// Called when the game starts or when spawned
void AFirstEquipmentItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstEquipmentItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirstEquipmentItem::OnMyCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->IncreaseAttackDamage(3.0f);
		Destroy();
	}
}

