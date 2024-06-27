// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckDistance.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC.h"
UBTService_CheckDistance::UBTService_CheckDistance()
{
	NodeName = TEXT("Distance");
	Interval = 1.0f;
}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;
	if (APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		auto Blackboard = OwnerComp.GetBlackboardComponent();
		float BBDistance = Blackboard->GetValueAsFloat(FName("Distance"));
		float BBAttackDistance = Blackboard->GetValueAsFloat(FName("AttackDistance"));
		FVector distance = Player->GetActorLocation() - ControllingPawn->GetActorLocation();
		if (distance.Size() < BBAttackDistance)
		{
			Blackboard->SetValueAsObject(FName("Player"), Player);
			ANPC* npc = Cast<ANPC>(Player);
			npc->Attack();
		/*	if (distance.Size() < BBAttackDistance)
			{
				Blackboard->SetValueAsBool(FName("IsAttack"), true);
			}*/
		}
	}
}
