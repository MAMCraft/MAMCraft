// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "EnemyBlazePawn.generated.h"

UCLASS()
class LSJ_API AEnemyBlazePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBlazePawn();

	UBehaviorTree* GetBehaviorTree() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))
	class UBehaviorTree* bTree;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))
	class UBlackboardData* bData;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	USkeletalMeshComponent* skMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "COLLISIONCAPCULE")
	UBoxComponent* boxComponent;
	UPROPERTY(VisibleAnywhere, Category = "MOVEMENT")
	UFloatingPawnMovement* movement;

};
