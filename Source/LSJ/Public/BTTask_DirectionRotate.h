// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DirectionRotate.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTask_DirectionRotate : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	float currentTime;
	APawn* ControllingPawn;
	double newYaw;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
public:
	UBTTask_DirectionRotate();
};
