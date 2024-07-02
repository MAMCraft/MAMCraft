// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AttackRangeState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

UENUM(BlueprintType)


void UBTService_AttackRangeState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector target = (OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("PlayerPos")));
	float distance = (target - controllingPawn->GetActorLocation()).Length();

	if (attackMinRadius > distance)
	{
		IsTooClose = true;
		IsAttackRange = true;
	}
	else if (attackMaxRadius < distance)
	{
		IsTooClose = false;
		IsAttackRange = false;
	}
	else //InAttackRange
	{
		IsAttackRange = true;
		IsTooClose = false;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("InAttackRange"), IsAttackRange);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsTooClose"), IsTooClose);
		
	DrawDebugSphere(controllingPawn->GetWorld(), controllingPawn->GetActorLocation(), attackMinRadius, 16, FColor::Yellow, false, 0.2f);
	DrawDebugSphere(controllingPawn->GetWorld(), controllingPawn->GetActorLocation(), attackMaxRadius, 16, FColor::Yellow, false, 0.2f);
}
