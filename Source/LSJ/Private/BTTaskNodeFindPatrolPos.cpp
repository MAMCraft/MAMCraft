// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskNodeFindPatrolPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

UBTTaskNodeFindPatrolPos::UBTTaskNodeFindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTaskNodeFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn =OwnerComp.GetAIOwner()->GetPawn();
	UE_LOG(LogTemp, Display, TEXT("%s"), (ControllingPawn==nullptr)?TEXT("ControllingPawn 없음"):TEXT("ControllingPawn 있음"));
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
	FVector Origin = ControllingPawn->GetActorLocation();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	UE_LOG(LogTemp, Display, TEXT("%s"), (NavSystem == nullptr) ? TEXT("NavSystem 없음") : TEXT("NavSystem 있음"));
	if (nullptr == NavSystem) return EBTNodeResult::Failed;

	//FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("HomePos"));
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		float zValue = ControllingPawn->GetActorLocation().Z + NextPatrol.Location.Z;
		float xValue = ControllingPawn->GetActorLocation().X + NextPatrol.Location.X;
		FVector v = FVector(xValue, ControllingPawn->GetActorLocation().Y, zValue);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("PatrolPos"), NextPatrol);

		UE_LOG(LogTemp, Display, TEXT("EBTNodeResult::Succeeded"));
		return EBTNodeResult::Succeeded;
	}
	UE_LOG(LogTemp, Display, TEXT("EBTNodeResult::Failed"));
	return EBTNodeResult::Failed;
}
