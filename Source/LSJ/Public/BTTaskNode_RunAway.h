// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RunAway.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTaskNode_RunAway : public UBTTaskNode
{
	GENERATED_BODY()
private:
	float currentTime=0.f;
	UPROPERTY(EditAnywhere, Category = "MOVINGTIME")
	float movingTime = 1.f;
		UPROPERTY(EditAnywhere, Category = "MOVINGTIME")
	float runAwaySpeed = 300.f;
public:
	UBTTaskNode_RunAway();
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
