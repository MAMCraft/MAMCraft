// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DirectionRotate.h"
#include "AIController.h"
#include <Kismet/KismetMathLibrary.h>
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DirectionRotate::UBTTask_DirectionRotate()
{
	bNotifyTick = true;
}
void UBTTask_DirectionRotate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FVector start = ControllingPawn->GetActorLocation();
	start.Z = 0;
	FVector end;
	if(BlackboardKey.IsSet())
		end = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
	else
	{
		APawn* target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));
		FVector oppositeDirection = target->GetActorLocation() - ControllingPawn->GetActorLocation();
		oppositeDirection.Z = 0;
		FVector bActorLocation = ControllingPawn->GetActorLocation() + -1.0f * oppositeDirection.GetSafeNormal2D() * 500.0f * DeltaSeconds;
		end = bActorLocation;//OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("RunAwayPos"));
	}

	end.Z = 0;
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(start, end);
	if (ControllingPawn->GetActorRotation().Yaw > lookRotation.Yaw - 5.0 && lookRotation.Yaw + 5.0 > ControllingPawn->GetActorRotation().Yaw)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	FRotator newRotation = FRotator(ControllingPawn->GetActorRotation().Pitch, lookRotation.Yaw, ControllingPawn->GetActorRotation().Roll);

	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), newRotation, DeltaSeconds, 5.f));

}

EBTNodeResult::Type UBTTask_DirectionRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	return EBTNodeResult::InProgress;
}
