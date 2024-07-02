// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_BlazeAttack.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTaskNode_BlazeAttack : public UBTTaskNode
{
	GENERATED_BODY()


public:
	UBTTaskNode_BlazeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
