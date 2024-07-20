// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_LookTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/KismetMathLibrary.h>

UBTTaskNode_LookTarget::UBTTaskNode_LookTarget()
{
	bNotifyTick = true;
}
void UBTTaskNode_LookTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FVector start = ControllingPawn->GetActorLocation();
	start.Z = 0;
	if (target == nullptr)
		return;
	FVector end = target->GetActorLocation();
	end.Z = 0;
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(start, end);
	//if (ControllingPawn->GetActorRotation().Yaw > lookRotation.Yaw - 5.0 && lookRotation.Yaw + 5.0 > ControllingPawn->GetActorRotation().Yaw)
		//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	if (currentTime > 1.0f)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	FRotator newRotation = FRotator(ControllingPawn->GetActorRotation().Pitch, lookRotation.Yaw, ControllingPawn->GetActorRotation().Roll);
	currentTime += DeltaSeconds;
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), newRotation, DeltaSeconds, 5.f));
}

EBTNodeResult::Type UBTTaskNode_LookTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));
	return EBTNodeResult::InProgress;
}
