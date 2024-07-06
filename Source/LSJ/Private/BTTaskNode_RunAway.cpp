// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RunAway.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	APawn* target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));

	FVector oppositeDirection = target->GetActorLocation() - ControllingPawn->GetActorLocation();
	oppositeDirection.Z = 0;
	UE_LOG(LogTemp, Log, TEXT("oppositeDirection %f %f"), oppositeDirection.X, oppositeDirection.Y);
	FVector bActorLocation = ControllingPawn->GetActorLocation() + -1.0f * oppositeDirection.GetSafeNormal2D() * runAwaySpeed * DeltaSeconds;

	ControllingPawn->SetActorLocation(bActorLocation);
}
EBTNodeResult::Type UBTTaskNode_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("WasRunAway"), true);
	return EBTNodeResult::InProgress;
}
