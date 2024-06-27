// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckAttackDistance::UBTService_CheckAttackDistance()
{
	NodeName = "Check Attack Distance";
}

void UBTService_CheckAttackDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	APawn* player = Cast<APawn>(Blackboard->GetValueAsObject(FName("Player")));
	float BBAttackDistance = Blackboard->GetValueAsFloat(FName("AttackDistance"));
	FVector distance = player->GetActorLocation() - ControllingPawn->GetActorLocation();
	if (distance.Size() < BBAttackDistance)
	{
		Blackboard->SetValueAsBool(FName("IsAttack"),true);
		UE_LOG(LogTemp, Display, TEXT("IsAttack"));
	}
}

