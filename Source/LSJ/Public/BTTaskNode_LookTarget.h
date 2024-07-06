// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_LookTarget.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTaskNode_LookTarget : public UBTTaskNode
{
	GENERATED_BODY()

	APawn* ControllingPawn;
	APawn* target;
	double newYaw;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
public:
	UBTTaskNode_LookTarget();
};
