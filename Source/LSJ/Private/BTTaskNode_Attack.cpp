// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"
#include "AIController.h"
#include "EnemyAnimationAttackInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("IsAttacking")))
	{
		return EBTNodeResult::Failed;
	}
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	IEnemyAnimationAttackInterface* attackPawn = Cast<IEnemyAnimationAttackInterface>(ControllingPawn);
	if (attackPawn)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAttacking"), true);
		attackPawn->Attack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
