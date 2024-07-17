// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpGate.h"
#include "MAMCGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "MAMCGameModeBase.h"
#include "PlayerCharacter.h"
// Sets default values
AWarpGate::AWarpGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOXCOMPONENT"));
	boxCollision->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	boxCollision->SetBoxExtent(FVector(501.f, 501.f, 501.f));
	boxCollision->SetupAttachment(RootComponent);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->SetupAttachment(boxCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/LSJ/Resource/MapResource/Potal/SM_MERGED_StaticMeshActor.SM_MERGED_StaticMeshActor'"));
	if (sphereMesh.Succeeded())
	{
		mesh->SetStaticMesh(sphereMesh.Object);
	}
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWarpGate::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWarpGate::BeginPlay()
{
	Super::BeginPlay();
	
}
void AWarpGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("On Overlap Begin... Other Actor Name: %s"), *OtherActor->GetName()));
	if (OtherActor->ActorHasTag("Player"))
		if (state == 0)
		{
			/*	UMAMCGameInstance* instanceMAMC = Cast<UMAMCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
				instanceMAMC->SaveInventory(player->inventoryComponent->Items);
				instanceMAMC->Save();*/
			APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
			UMAMCGameInstance* gi = Cast<UMAMCGameInstance>(GetGameInstance());
			gi->Save(player->inventoryComponent->Items);
			UGameplayStatics::OpenLevel(this, TEXT("HMap"));
		}

		else if (state == 1)
		{
			AMAMCGameModeBase* gamemode = Cast<AMAMCGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			gamemode->VisibleSuccessWidget();
			UGameplayStatics::SetGamePaused(GetWorld(),true);
		}
		
}
// Called every frame
void AWarpGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

