// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeFindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTaskNodeFindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNodeFindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};