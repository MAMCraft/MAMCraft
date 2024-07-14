// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_BlazeAttack.h"
#include "AIController.h"
#include "EnemyAnimationAttackInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTaskNode_BlazeAttack::UBTTaskNode_BlazeAttack()
{
}

EBTNodeResult::Type UBTTaskNode_BlazeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector targetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerPos"));
	TArray <FVector> attackLocaction;
	attackLocaction.Emplace(targetLocation);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem) return EBTNodeResult::Failed;

	//2번 반복해서 attackLocaction index 1,2를 만든다.
	for(int i = 0; i < 2; i++)
	{
		FNavLocation NextPatrol;
		if (NavSystem->GetRandomPointInNavigableRadius(targetLocation, 500.0f, NextPatrol))
		{
			FVector randomLocation = NextPatrol.Location;
			randomLocation.Z = ControllingPawn->GetActorLocation().Z;
			attackLocaction.Emplace(randomLocation);
		}
	}
	
	IEnemyAnimationAttackInterface* attackPawn = Cast<IEnemyAnimationAttackInterface>(ControllingPawn);
	if (attackPawn)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAttacking"), true);
		attackPawn->Attack(attackLocaction);
		//OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("WasRunAway"),false);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
