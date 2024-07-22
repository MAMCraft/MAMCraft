// Fill out your copyright notice in the Description page of Project Settings.


#include "ClickMovePlayerController.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <PlayerCharacter.h>
#include <TreasureChest.h>
#include <IncreaseHPItem.h>
#include <InventoryComponent.h>
#include "GameFramework/PlayerController.h"
AClickMovePlayerController::AClickMovePlayerController()
{
	bShowMouseCursor = true;
	//UE_LOG(LogTemp, Log, TEXT("Mouse"));
}

void AClickMovePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AClickMovePlayerController::InputLeftMouseButtonPressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AClickMovePlayerController::InputLeftMouseButtonReleased);
	InputComponent->BindAction("Posion", IE_Pressed, this, &AClickMovePlayerController::UsePosionItem);
}

void AClickMovePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bClickLeftMouse)
	{
		bClickLeftMouse = false;
		MoveToMouseCursor();
	}
	if (bIsEnemyHere)
	{
		float distance = (EnemyLocation - PlayerCharacter2->GetActorLocation()).Length();
		if (distance < 170.f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ATTACK!!!!!!!!!!!!!!!!!!!!!!!!!!"))
			FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter2->GetActorLocation(), EnemyLocation);
			FRotator newRotation = FRotator(PlayerCharacter2->GetActorRotation().Pitch, lookRotation.Yaw, PlayerCharacter2->GetActorRotation().Roll);
			PlayerCharacter2->SetActorRotation(newRotation);
			PlayerCharacter2->comboAttack();
			bIsEnemyHere = false;
		}
	}
		

}

void AClickMovePlayerController::BeginPlay()
{
	//PlayerCharacter = Cast<ACharacter>(GetPawn());
	PlayerCharacter2 = Cast<APlayerCharacter>(GetPawn());
	PlayerCharacter2->SetController(this);
}

void AClickMovePlayerController::UsePosionItem()
{
	//if (PlayerCharacter2->inventoryComponent->Items.IsEmpty())
	//	return;
	PlayerCharacter2->IncreaseHP(80);
	//PlayerCharacter2->inventoryComponent->RemoveItem(PlayerCharacter2->inventoryComponent->Items[0]);
}

void AClickMovePlayerController::InputLeftMouseButtonPressed()
{
	bClickLeftMouse = true;
}

void AClickMovePlayerController::InputLeftMouseButtonReleased()
{
	bClickLeftMouse = false;
}

void AClickMovePlayerController::SetNewDestination(const FVector Destination)
{
	APawn* const MyPawn = GetPawn();

	if (MyPawn)
	{
		float const Distance = FVector::Dist(Destination, MyPawn->GetActorLocation());
		if (Distance > 120.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
		}
	}
}

void AClickMovePlayerController::SetNewMove(const FVector Dest)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Dest);
}

void AClickMovePlayerController::MoveToMouseCursor()
{
	bIsEnemyHere = false;
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);
	AActor* Target = hit.GetActor();
	if (Target == nullptr)
		return;
	// tag 넣어서 아이템별 각 기능 넣어주기
	if (Target->ActorHasTag(FName(TEXT("ItemBowBubble"))))
	{
		// do something
		//UE_LOG(LogTemp, Warning, TEXT("ItemBowBubble"));
		AItem* item = Cast<AItem>(Target);
		PlayerCharacter2->inventoryComponent->AddItem(item);
		Target->Destroy();

	}
	else if (Target->ActorHasTag(FName(TEXT("ItemBowBasic"))))
	{
		// do something
		//UE_LOG(LogTemp, Warning, TEXT("ItemBowBasic"));
		Target->Destroy();

	}
	else if (Target->ActorHasTag(FName(TEXT("ItemSword"))))
	{
		// do something
		//UE_LOG(LogTemp, Warning, TEXT("ItemSword"));
		Target->Destroy();

	}
	else if (Target->ActorHasTag(FName(TEXT("ArrowItem"))))
	{
		// do something
		PlayerCharacter2->inventoryComponent->AddArrow(5);
		//UE_LOG(LogTemp, Warning, TEXT("Arrow Item"));
		Target->Destroy();

	}
	else if (Target->ActorHasTag(FName(TEXT("IncreaseHPItem"))))
	{
		//LSJ 인벤토리
		AItem* item = Cast<AItem>(Target);
		PlayerCharacter2->IncreaseHP(10);
		Target->Destroy();

	}
	else if (Target->ActorHasTag(FName(TEXT("FirstEquipmentItem"))))
	{
		// do something
		//UE_LOG(LogTemp, Warning, TEXT("First Equipment Item"));
		Target->Destroy();

	}
	else if (Target->ActorHasTag(FName(TEXT("SecondEquipmentItem"))))
	{
		// do something
		//UE_LOG(LogTemp, Warning, TEXT("Second Equipment Item"));
		Target->Destroy();
	}
	else if (Target->ActorHasTag(FName(TEXT("Enemy"))))
	{
		EnemyLocation = Target->GetActorLocation();
		bIsEnemyHere = true;
		SetNewDestination(hit.ImpactPoint);
	}
	else if (Target->ActorHasTag(FName("TreasureChest")))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Item Spawn22222222222222222222222222222"))
			ATreasureChest* Chest = Cast<ATreasureChest>(Target);
		if (Chest)
		{
			Chest->OnChestClicked();
		}
	}

	if (hit.bBlockingHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Iclick"))
			SetNewDestination(hit.ImpactPoint);
	}
}

