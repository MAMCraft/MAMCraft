// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LSJ.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnemyZombie.generated.h"

UCLASS()
class LSJ_API AEnemyZombie : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "MESH")
	USkeletalMeshComponent* skMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "CAPSULE")
	UCapsuleComponent* capsuleComponent;
	UPROPERTY(VisibleAnywhere, Category = "ROOT")
	USceneComponent* rootComp;
	UPROPERTY(VisibleAnywhere, Category = "ROOT")
	FVector rootCompScale;
	UPROPERTY(VisibleAnywhere, Category = "MOVEMENT")
	UFloatingPawnMovement* movement;

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* newController) override;
public:
	UFUNCTION(BlueprintCallable)
	const FVector& GetNextPatrolPoint();

public:
	UPROPERTY(EditAnywhere,Category="PATROL",Meta=(MakeEditWidget = true))
	TArray<FVector> localPatrolPoints;
	TArray<FVector> worldPatrolPoints;


};
