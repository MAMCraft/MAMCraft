// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClickMovePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JYS_API AClickMovePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AClickMovePlayerController();
	bool bClickLeftMouse;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UAnimMontage* runMontage;

protected:

public:
	void InputLeftMouseButtonPressed();
	void InputLeftMouseButtonReleased();

	void SetNewDestination(const FVector Destination);
	void SetNewMove(const FVector Dest);

	void MoveToMouseCursor();
	
	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere)
	//class ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	class APlayerCharacter* PlayerCharacter2;

	FVector EnemyLocation;

	bool bIsEnemyHere;

	AActor* OnChestClicked;

	void UsePosionItem();
};
