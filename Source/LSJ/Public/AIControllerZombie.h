// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerZombie.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API AAIControllerZombie : public AAIController
{
	GENERATED_BODY()

	FTimerHandle MovementTimerHandle;

public:
	AAIControllerZombie();
	
	


	/// 
	virtual void OnPossess(APawn* InPawn) override;
	virtual void PostInitializeComponents() override;
	void RunAI();

	//static const FName HomePosKey;
	//static const FName PatrolPosKey;
private:

	FTimerHandle repeatTimerHandle;
	float repeatInterval;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BEHAVIOR", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* bTAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BEHAVIOR", meta = (AllowPrivateAccess = "true"))
	class UBlackboardData* bBAsset;


};
