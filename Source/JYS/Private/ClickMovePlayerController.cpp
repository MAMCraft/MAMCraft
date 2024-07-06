// Fill out your copyright notice in the Description page of Project Settings.


#include "ClickMovePlayerController.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <PlayerCharacter.h>


AClickMovePlayerController::AClickMovePlayerController()
{
	bShowMouseCursor = true;
	UE_LOG(LogTemp, Log, TEXT("Mouse"));
}

void AClickMovePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AClickMovePlayerController::InputLeftMouseButtonPressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AClickMovePlayerController::InputLeftMouseButtonReleased);
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
			UE_LOG(LogTemp, Warning, TEXT("ATTACK!!!!!!!!!!!!!!!!!!!!!!!!!!"))
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
	PlayerCharacter = Cast<ACharacter>(GetPawn());
	PlayerCharacter2 = Cast<APlayerCharacter>(GetPawn());
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

void AClickMovePlayerController::MoveToMouseCursor()
{
	bIsEnemyHere = false;
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);
	AActor* Target = hit.GetActor();
	if (Target == nullptr)
		return;
	// tag 넣어서 아이템별 각 기능 넣어주기
	if (Target->ActorHasTag(FName(TEXT("ArrowItem"))))
	{
		// do something
		UE_LOG(LogTemp,Warning,TEXT("Arrow Item"));
		Target->Destroy();

	}else if (Target->ActorHasTag(FName(TEXT("IncreaseHPItem"))))
	{
		// do something
		UE_LOG(LogTemp, Warning, TEXT("Increase HP Item"));
		Target->Destroy();

	}else if (Target->ActorHasTag(FName(TEXT("FirstEquipmentItem"))))
	{
		// do something
		UE_LOG(LogTemp, Warning, TEXT("First Equipment Item"));
		Target->Destroy();

	}else if (Target->ActorHasTag(FName(TEXT("SecondEquipmentItem"))))
	{
		// do something
		UE_LOG(LogTemp, Warning, TEXT("Second Equipment Item"));
		Target->Destroy();
	}
	else if (Target->ActorHasTag(FName(TEXT("Enemy"))))
	{
		EnemyLocation = Target->GetActorLocation();
		bIsEnemyHere = true;
		SetNewDestination(hit.ImpactPoint);
	}

	if (hit.bBlockingHit)
	{
		SetNewDestination(hit.ImpactPoint);
	}
}

