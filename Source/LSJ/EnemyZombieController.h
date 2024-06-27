// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyZombieController.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API AEnemyZombieController : public AAIController
{
	GENERATED_BODY()
	
	FTimerHandle MovementTimerHandle;
protected:
	virtual void BeginPlay() override;
public:
	AEnemyZombieController();
	virtual void Tick(float deltaSeconds) override;
	virtual void OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result) override;

	UFUNCTION()
	void GoToNextPatrolPoint();

	void SetNewDestination(const FVector destination);


	/// 
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
