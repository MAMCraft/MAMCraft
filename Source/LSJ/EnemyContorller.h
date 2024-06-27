// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyContorller.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API AEnemyContorller : public AAIController
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};
