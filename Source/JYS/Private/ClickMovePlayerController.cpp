// Fill out your copyright notice in the Description page of Project Settings.


#include "ClickMovePlayerController.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
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
		MoveToMouseCursor();
		UAnimInstance* AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && runMontage)
		{
			AnimInstance->Montage_Play(runMontage);
		}
	}
}

void AClickMovePlayerController::BeginPlay()
{
	PlayerCharacter = Cast<ACharacter>(GetPawn());
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
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);

	if (hit.bBlockingHit)
	{
		SetNewDestination(hit.ImpactPoint);
	}
}

