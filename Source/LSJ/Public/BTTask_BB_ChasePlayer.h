// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EnemyZombiePawn.h"
#include "BTTask_BB_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTask_BB_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_BB_ChasePlayer(FObjectInitializer const& object_initializer);
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
