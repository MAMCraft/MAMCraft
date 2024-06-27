// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DistanceAttack_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class MAMCRAFT_API UDistanceAttack_BTTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDistanceAttack_BTTask();
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PlayerKey;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
