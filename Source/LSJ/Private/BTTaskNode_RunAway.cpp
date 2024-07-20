// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RunAway.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIControllerBlaze.h"
UBTTaskNode_RunAway::UBTTaskNode_RunAway()
{
	NodeName = "UBTTaskNode_RunAway";
	bNotifyTick = true;
}
void UBTTaskNode_RunAway::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	currentTime += DeltaSeconds;
	if (currentTime > movingTime)
	{
		currentTime = 0;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsGround"), true);
		IsGround = true;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (ControllingPawn == nullptr)
		return;
	FVector bActorLocation;
	APawn* target;
	FVector oppositeDirection;
	//땅이 있는지 체크
	FHitResult hitResult;
	start = ControllingPawn->GetActorLocation();
	FVector end = start + ControllingPawn->GetRootComponent()->GetForwardVector() * 100.0f + ControllingPawn->GetRootComponent()->GetUpVector() * -1.f * 500.0f;
	FCollisionQueryParams Params(NAME_None, true, ControllingPawn);
	if (!GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, Params))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsGround"), false);
	}
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("IsGround")))
	{
		//가려는 방향에 땅이 있을 경우 그대로 이동
		/*DrawDebugLine(
			GetWorld(),
			start,
			start + ControllingPawn->GetRootComponent()->GetForwardVector() * 100.0f + ControllingPawn->GetRootComponent()->GetUpVector() * -1.f * 100.0f,
			FColor(255, 0, 0),
			false, -1, 0,
			12.333
		);*/
		target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));
		if (target == nullptr)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}
		oppositeDirection = target->GetActorLocation() - ControllingPawn->GetActorLocation();
		oppositeDirection.Z = 0;
		bActorLocation = ControllingPawn->GetActorLocation() + -1.0f * oppositeDirection.GetSafeNormal() * runAwaySpeed * DeltaSeconds;
	}
	else
	{
		//move to 
		//UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), start, bActorLocation);
		//FAIMoveRequest moveRequest;
		//moveRequest.SetGoalLocation(FVector(NextPatrol.Location.X, NextPatrol.Location.Y, ControllingPawn->GetActorLocation().Z));
		//moveRequest.SetAcceptanceRadius(1.0f);
		//AAIControllerBlaze* blazeController = Cast<AAIControllerBlaze>(ControllingPawn->GetController());
		//blazeController->MoveTo(moveRequest, nullptr); 

		//가려는 방향에 땅이 없을 경우
		//랜덤 이동
		oppositeDirection = (NextPatrol.Location - ControllingPawn->GetActorLocation());
		oppositeDirection.Z = 0;
		bActorLocation = ControllingPawn->GetActorLocation() + oppositeDirection.GetSafeNormal() * runAwaySpeed * DeltaSeconds;
	}
	//로그
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hit Actor Name: %d"), IsGround));
	ControllingPawn->SetActorLocation(bActorLocation,true);
	//AAIController* aicontroller =Cast<AAIController>(ControllingPawn->GetController());
	//aicontroller->MoveToLocation(NextPatrol);
	//OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RunAwayPos"), bActorLocation);

	//aicontroller->MoveToLocation(NextPatrol,10.0f);

}
EBTNodeResult::Type UBTTaskNode_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("WasRunAway"), true);
	ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return EBTNodeResult::Failed;
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem) return EBTNodeResult::Failed;
	start = ControllingPawn->GetActorLocation();
	if (NavSystem->GetRandomPointInNavigableRadius(start, 500.0f, NextPatrol))
	{
		NextPatrol.Location.Z = ControllingPawn->GetActorLocation().Z;
	}
	return EBTNodeResult::InProgress;
}
