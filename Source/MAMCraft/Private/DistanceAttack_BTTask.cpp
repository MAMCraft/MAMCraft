// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceAttack_BTTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UDistanceAttack_BTTask::UDistanceAttack_BTTask()
{
	NodeName = "Check Attack Distance";
}

EBTNodeResult::Type UDistanceAttack_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	APawn* player = Cast<APawn>(Blackboard->GetValueAsObject(FName("Player")));
	float BBAttackDistance = Blackboard->GetValueAsFloat(FName("AttackDistance"));
	FVector distance = player->GetActorLocation() - ControllingPawn->GetActorLocation();
	if (distance.Size() < BBAttackDistance)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
